#ifndef TRANSMISSIVE
#define TRANSMISSIVE
#include "material.h"

class Transmissive : public Material {

public:
    double mu_t;

    Transmissive(double mu_t);
    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const override;

    bool hasSpecular() const override;
    virtual bool hasTransmission() const override;
    virtual bool hasDiffuseOrGlossy() const override;
    double getIndexOfRefraction() const;
    Vector3D getDiffuseCoefficient() const override;
};

#endif // TRANSMISSIVE
