#include "depthshader.h"
#include "../core/utils.h"


DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_) :
   color(color_), maxDist(maxDist_), Shader(bgColor_)
{ }

Vector3D DepthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection its;
    double hitDist;

    if (Utils::getClosestIntersection(r, objList, its))
    {    
        Vector3D color;

        hitDist = sqrt((its.itsPoint.x - r.d.x) * (its.itsPoint.x - r.d.x) +
                      (its.itsPoint.y - r.d.y) * (its.itsPoint.y - r.d.y) +
                      (its.itsPoint.z - r.d.z) * (its.itsPoint.z - r.d.z));

        color = Vector3D(0, (double)(1 - hitDist / maxDist), 0);

        return color;
    }
    else
        return bgColor;
}