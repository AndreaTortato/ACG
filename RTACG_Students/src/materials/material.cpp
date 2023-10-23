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

Vector3D Material::getDiffuseCoefficient() const
{
    std::cout   << "Warning!"
                << "Calling \"Material::getDiffuseCoefficient()\""
                << "for a non - diffuse or non - glossy material"
                << std::endl;
    return Vector3D(-1);
}