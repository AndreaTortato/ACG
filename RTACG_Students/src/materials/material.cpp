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