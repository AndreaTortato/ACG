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

        for (size_t lsIdx = 0; lsIdx < lsList.size(); lsIdx++) {
            const PointLightSource ls = lsList[lsIdx];
            Vector3D l = ls.getPosition() - its.itsPoint;
            Vector3D w0 = -r.d;
            Vector3D wi = l.normalized();
            Vector3D reflectance = its.shape->getMaterial().getReflectance(its.normal, w0, wi);
            color += reflectance * ls.getIntensity(its.itsPoint);
        }
        return color;  
    }
    else 
        return bgColor;
}