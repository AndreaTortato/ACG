#ifndef DEPTHSHADER
#define DEPTHSHADER

#include "shader.h"


class DepthShader : public Shader
{
public:

    DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_);

    Vector3D computeColor(const Ray &r,
                             const std::vector<Shape*> &objList,
                             const std::vector<PointLightSource> &lsList) const;

private:
    double maxDist;
    Vector3D color;
};

#endif // DEPTHSHADER