#ifndef PHONG
#define PHONG
#include "material.h"

class Phong : public Material {

public:
    Vector3D kd; //diffuse coeffiecient
    Vector3D ks; //specular coefficient
    double alpha; //shininess-roughness

    Phong(Vector3D kd, Vector3D ks, double alpha);
    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const override;

    bool hasSpecular() const override;
    virtual bool hasTransmission() const override;
    virtual bool hasDiffuseOrGlossy() const override;
    Vector3D getDiffuseCoefficient() const override;
};

#endif // PHONG