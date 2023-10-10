#ifndef DIRECTSHADER
#define DIRECTSHADER

#include "shader.h"


class DirectShader : public Shader
{
public:

    DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_);

    Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lightList) const;

private:
    double maxDist;
    Vector3D color;
};

#endif // DIRECTSHADER
