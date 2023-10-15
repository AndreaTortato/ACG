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
        //Mirror shader
        else if (its.shape->getMaterial().hasSpecular())
        {
            Vector3D w0 = -r.d;
            Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
            Ray reflectedRay = Ray(its.itsPoint, wr, r.depth + 1, Epsilon, INFINITY);
            color = computeColor(reflectedRay, objList, lsList);
        }
        //Transmissive shader
        else if (its.shape->getMaterial().hasTransmission())
        {
            double eta, cosThetaI, cosThetaT_out, cosThetaT;

            if(!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT_out))
            {
                Vector3D wt = Utils::computeTransmissionDirection(r, its.normal, eta, cosThetaI, cosThetaT);
                Ray refractedRay = Ray(its.itsPoint, wt, r.depth + 1);
                color = computeColor(refractedRay, objList, lsList);
            }
        }
        return color;  
    }
    else 
        return bgColor;
}