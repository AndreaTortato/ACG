#include "depthshader.h"
#include "../core/utils.h"

DepthShader::DepthShader() :
   color(Vector3D(1, 0, 0))
{ }

DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_) :
   color(color_), maxDist(maxDist_), Shader(bgColor_)
{ }

Vector3D DepthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection its;
    double hitDist;

    if (Utils::getClosestIntersection(r, objList, its))
    {
        hitDist = sqrt((its.itsPoint.x - r.d.x) * (its.itsPoint.x - r.d.x) +
                      (its.itsPoint.y - r.d.y) * (its.itsPoint.y - r.d.y) +
                      (its.itsPoint.z - r.d.z) * (its.itsPoint.z - r.d.z));

        //TASK 3
        Vector3D color = Vector3D(0, (double)(1 - hitDist / maxDist), 0);

        //TASK 4
        Vector3D color = (its.normal + Vector3D(1.0, 1.0, 1.0)) / 2;

        return color;
    }
    else
        return bgColor;
}