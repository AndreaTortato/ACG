#include "globalshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"

GlobalShader::GlobalShader(Vector3D ambient_):
	ambient(ambient_)
{}

Vector3D GlobalShader::computeColor(const Ray& r,
    const std::vector<Shape*>& objList,
    const std::vector<PointLightSource>& lsList) const {

    Intersection its;

    if (Utils::getClosestIntersection(r, objList, its))
    {
        Vector3D Ldir;
        Vector3D Lind;
        Vector3D Lo; //same as color in pther shaders

        //Phong shader
        if (its.shape->getMaterial().hasDiffuseOrGlossy())
        {
            //calculate direct light
            for (size_t lsIdx = 0; lsIdx < lsList.size(); lsIdx++) {

                const PointLightSource ls = lsList[lsIdx];
                Vector3D l = ls.getPosition() - its.itsPoint;;
                Vector3D w0 = -r.d;
                Vector3D wi = l.normalized();

                Ray shadowray = Ray(its.itsPoint, wi, r.depth + 1, Epsilon, INFINITY);
                shadowray.maxT = (its.itsPoint - ls.getPosition()).length();

                if (!Utils::hasIntersection(shadowray, objList))
                {
                    Vector3D reflectance = its.shape->getMaterial().getReflectance(its.normal, w0, wi);
                    Ldir += reflectance * ls.getIntensity(its.itsPoint);
                }
            }

            HemisphericalSampler hSampler = HemisphericalSampler();            
            int maxDepth = 8;
            int nSamples = 10;
            Vector3D Li, Li_n, Li_r;

            //calculate indirect light (3.2.2)
            if (r.depth == 0)
            {
                Vector3D wj;

                for (int j = 0; j < nSamples; j++)
                {
                    wj = hSampler.getSample(its.normal);
                    Ray randomRay = Ray(its.itsPoint, wj, r.depth + 1, Epsilon, INFINITY);

                    Li = computeColor(randomRay, objList, lsList);
                    Lind += Li / (2 * 3.141014 * nSamples);
                }
                
            }
            else if (r.depth == maxDepth)
            {
                //add indirect lighting (3.2.1)
                Lind = its.shape->getMaterial().getDiffuseCoefficient() * ambient; 
            }
            else
            {
                Vector3D wo = -r.d;
                Vector3D wr = its.normal * 2 * dot(its.normal, wo) - wo;
                Vector3D wn = its.normal;
                Ray Ray_n = Ray(its.itsPoint, wn, r.depth + 1, Epsilon, INFINITY);
                Ray Ray_r = Ray(its.itsPoint, wr, r.depth + 1, Epsilon, INFINITY);

                Li_n = Vector3D(0.5); //computeColor(Ray_n, objList, lsList);
                Li_r = Vector3D(0.5); //computeColor(Ray_r, objList, lsList);
                Lind += (Li_n + Li_r) / (4 * 3.141014);
            }
            printf("%i, ", r.depth);

            Lo += Ldir + Lind * 2;
        }

        //Mirror shader (3.1.1)
        else if (its.shape->getMaterial().hasSpecular())
        {
            Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
            Ray reflectedRay = Ray(its.itsPoint, wr, r.depth + 1, Epsilon, INFINITY);
            Lo += computeColor(reflectedRay, objList, lsList);
        }

        //Transmissive shader (3.1.2)
        else if (its.shape->getMaterial().hasTransmission())
        {
            double mu_t = its.shape->getMaterial().getIndexOfRefraction();

            Vector3D wo = -r.d;      //Vector from intersection point to view origin
            double cos = dot(its.normal, wo);
            double sin2alpha = 1 - cos * cos;
            double refract = 1 - std::pow(mu_t, 2) * sin2alpha;

            if (refract >= 0) {
                Vector3D wt = its.normal * (-sqrt(refract) + mu_t * cos) - wo * mu_t;

                Ray ray_refract(its.itsPoint, wt, r.depth + 1);

                Intersection new_its;
                if (Utils::getClosestIntersection(ray_refract, objList, new_its)) { // second intersection?

                    if (dot(new_its.normal, wt) > 0) { //there is refraction
                        mu_t = 1 / mu_t;
                        new_its.normal = -new_its.normal;

                        wo = -ray_refract.d;
                        cos = dot(new_its.normal, wo);
                        sin2alpha = 1 - cos * cos;
                        refract = 1 - std::pow(mu_t, 2) * sin2alpha;
                        wt = new_its.normal * (-sqrt(refract) + mu_t * cos) - wo * mu_t;

                        Ray ray_refract2(new_its.itsPoint, wt, ray_refract.depth + 1);
                        Lo += computeColor(ray_refract2, objList, lsList);
                    }
                }
                else { //no second intersection
                    Lo += computeColor(ray_refract, objList, lsList);
                }
            }
            else { //refract < 0, mirror

                Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
                Ray reflectedRay = Ray(its.itsPoint, wr, r.depth + 1, Epsilon, INFINITY);
                Lo += computeColor(reflectedRay, objList, lsList);
            }
        }
        
        return Lo;
    }
    else
        return bgColor;
}