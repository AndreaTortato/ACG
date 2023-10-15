#include "transmissive.h"

Transmissive::Transmissive(double mu_t)
{
    this->mu_t = mu_t;
}

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const
{
    return Vector3D(0.0);
}

bool Transmissive::hasSpecular() const
{
    return false;
}

bool Transmissive::hasTransmission() const
{
    return true;
}

bool Transmissive::hasDiffuseOrGlossy() const
{
    return false;
}