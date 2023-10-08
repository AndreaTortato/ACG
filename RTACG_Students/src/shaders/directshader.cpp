#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader() :
	color(Vector3D(1, 0, 0))
{ }

DirectShader::DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_) :
	color(color_), maxDist(maxDist_), Shader(bgColor_)
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lightList) const
{
    Intersection* its = new Intersection();

    if (Utils::getClosestIntersection(r, objList, *its)) {

        Vector3D color;

        for (int i = 0; i < lightList.size(); i++) {

            const PointLightSource light = lightList.at(i);

            Vector3D Li = light.getIntensity(its->itsPoint);
            Vector3D wi = (light.getPosition() - its->itsPoint).normalized();
            double maxT = (light.getPosition() - its->itsPoint).length();

            Ray wiRay = Ray(its->itsPoint, wi, 0, Epsilon, maxT);
            const Material phongmat = its->shape->getMaterial();
            Vector3D rp = phongmat.getReflectance(its->normal, (r.o - its->itsPoint).normalized(), wi);

            if (!Utils::hasIntersection(wiRay, objList)) color += Li * rp;

        }
        return color;
    }
    else
        return bgColor;
}