#ifndef GLOBALSHADER
#define GLOBALSHADER

#include "shader.h"


class GLobalShader : public Shader
{
public:

    GLobalShader(Vector3D color_, double maxDist_, Vector3D bgColor_);

    Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<PointLightSource>& lightList) const;

private:
    Vector3D ambient;
};

#endif // DIRECTSHADER