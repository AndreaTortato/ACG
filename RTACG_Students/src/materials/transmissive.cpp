#include "transmissive.h"

Transmissive::Transmissive(double mu_t)
{
    this->mu_t = mu_t;
}

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const
{
    //result = dot((std::sqrt(1 - std::pow(mu_t, 2) * (1 - std::pow(dot(wi, n), 2))) * dot(wi, n) - mu_t * dot(wi, n)), n) + dot(mu_t, wi);
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

double Transmissive::getIndexOfRefraction() const
{
    return mu_t;
}

Vector3D Transmissive::getDiffuseCoefficient() const
{
    return Vector3D(-1);
}