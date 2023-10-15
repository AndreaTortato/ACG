#include "phong.h"

Phong::Phong(Vector3D kd, Vector3D ks, double alpha)
{
    this->kd = kd;
    this->ks = ks;
    this->alpha = alpha;
}

Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const
{
    //ideal reflection
    Vector3D wr = Vector3D(2.0) * dot(n, wi) * n - wi;
    Vector3D reflection = kd * std::max(dot(wi, n), 0.0) + ks * pow(std::max(dot(wo, wr), 0.0), alpha);

    return reflection;
}

bool Phong::hasSpecular() const
{
    return false;
}

bool Phong::hasTransmission() const
{
    return false;
}

bool Phong::hasDiffuseOrGlossy() const
{
    return true;
}