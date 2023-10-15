#ifndef MIRROR
#define MIRROR
#include "material.h"

class Mirror : public Material {

public:
    Mirror();
    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const override;

    bool hasSpecular() const override;
    virtual bool hasTransmission() const override;
    virtual bool hasDiffuseOrGlossy() const override;
};

#endif // MIRROR
