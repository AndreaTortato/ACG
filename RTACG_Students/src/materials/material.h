#ifndef MATERIAL
#define MATERIAL

#include "../core/vector3d.h"

class Material
{
public:
    Material();

    virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
                                    const Vector3D &wi) const = 0;
    virtual bool hasSpecular() const = 0;
    virtual bool hasTransmission() const = 0;
    virtual bool hasDiffuseOrGlossy() const = 0;
    virtual double getIndexOfRefraction() const;
};

class Phong: public Material {

    //create different phong mateirals

    public:
        Vector3D kd; //diffuse coeffiecient
        Vector3D ks; //specular coefficient
        double alpha; //shininess-roughness
    
    Phong(Vector3D kd, Vector3D ks, double n);
    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const override;

    bool hasSpecular() const override;
    virtual bool hasTransmission() const override;
    virtual bool hasDiffuseOrGlossy() const override;
};  

#endif // MATERIAL
