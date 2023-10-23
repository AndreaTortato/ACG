#include "mirror.h"

Mirror::Mirror()
{}

Vector3D Mirror::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const
{
    Vector3D wr = Vector3D(2.0) * dot(n, wo) * n - wo;
    return wr;
}

bool Mirror::hasSpecular() const
{
    return true;
}

bool Mirror::hasTransmission() const
{
    return false;
}

bool Mirror::hasDiffuseOrGlossy() const
{
    return false;
}

Vector3D Mirror::getDiffuseCoefficient() const 
{
    return Vector3D(-1);
}