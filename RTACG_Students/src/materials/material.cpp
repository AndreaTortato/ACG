#include "material.h"

#include <iostream>

Material::Material()
{ }

double Material::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}

Phong::Phong(Vector3D kd, Vector3D ks, double alpha)
{
    this->kd = kd;
    this->ks = ks;
    this->alpha = alpha;
}

Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const
{
    //ideal reflection
    Vector3D wr = Vector3D(2) * dot(n, wi) * n - wi;
    Vector3D reflection = kd * dot(wi, n) + ks * pow(dot(wo, wr), alpha);

    return Vector3D(1.0, 1.0, 1.0);
}

bool Material::hasSpecular() const
{
    return false;
}

bool Material::hasTransmission() const
{
    return false;
}

bool Material::hasDiffuseOrGlossy() const
{
    return false;
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
    return false;
}