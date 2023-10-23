#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_) :
    color(color_), maxDist(maxDist_), Shader(bgColor_)
{ }


Vector3D DirectShader::computeColor(const Ray &r,
                             const std::vector<Shape*> &objList,
                             const std::vector<PointLightSource> &lsList) const{

    Intersection its;
    if (Utils::getClosestIntersection(r, objList, its))
    {
        Vector3D color;

        //Phong shader
        if (its.shape->getMaterial().hasDiffuseOrGlossy())
        {
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
                    color += reflectance * ls.getIntensity(its.itsPoint);
                }
            }
        }

        //Mirror shader (3.1.1)
        else if (its.shape->getMaterial().hasSpecular())
        {
            Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
            Ray reflectedRay = Ray(its.itsPoint, wr, r.depth + 1, Epsilon, INFINITY);
            color = computeColor(reflectedRay, objList, lsList);
        }

        //Transmissive shader (3.1.2)
        else if (its.shape->getMaterial().hasTransmission())
        {
                double mu_t = its.shape->getMaterial().getIndexOfRefraction();

                Vector3D wo = -r.d;      //Vector from intersection point to view origin
                double cos = dot(its.normal, wo);
                double sin2alpha = 1 - cos * cos;
                double refract = 1 - std::pow(mu_t,2) * sin2alpha;

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
                            refract = 1 - std::pow(mu_t,2) * sin2alpha;
                            wt = new_its.normal * (-sqrt(refract) + mu_t * cos) - wo * mu_t;

                            Ray ray_refract2(new_its.itsPoint, wt, ray_refract.depth + 1);
                            color += computeColor(ray_refract2, objList, lsList);
                        }
                    }
                    else { //no second intersection
                        color += computeColor(ray_refract, objList, lsList);
                    }
                }
                else { //refract < 0, mirror

                    Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
                    Ray reflectedRay = Ray(its.itsPoint, wr, r.depth + 1, Epsilon, INFINITY);
                    color = computeColor(reflectedRay, objList, lsList);

                }
        }
        return color;  
    }
    else 
        return bgColor;
}