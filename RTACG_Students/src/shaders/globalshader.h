#ifndef GLOBALSHADER
#define GLOBALSHADER

#include "shader.h"


class GlobalShader : public Shader
{
public:

    GlobalShader(Vector3D ambient_);

    Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lightList) const;

private:
    Vector3D ambient;
};

#endif // GLOBALSHADER