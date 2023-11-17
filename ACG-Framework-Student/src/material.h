#ifndef MATERIAL_H
#define MATERIAL_H

#include "framework.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "extra/hdre.h"

class Material {
public:

	Shader* shader = NULL;
	Texture* texture = NULL;
	vec4 color;

	virtual void setUniforms(Camera* camera, Matrix44 model) = 0;
	virtual void render(Mesh* mesh, Matrix44 model, Camera * camera) = 0;
	virtual void renderInMenu() = 0;
};

class StandardMaterial : public Material {
public:

	StandardMaterial();
	~StandardMaterial();

	void setUniforms(Camera* camera, Matrix44 model);
	void render(Mesh* mesh, Matrix44 model, Camera * camera);
	void renderInMenu();
};

class WireframeMaterial : public StandardMaterial {
public:

	WireframeMaterial();
	~WireframeMaterial();

	void render(Mesh* mesh, Matrix44 model, Camera * camera);
};

// TODO: Derived class VolumeMaterial
class VolumeMaterial : public Material {
public:

	VolumeMaterial();
	~VolumeMaterial();

	void setUniforms(Camera* camera, Matrix44 model);
	void render(Mesh* mesh, Matrix44 model, Camera* camera);
	void renderInMenu();

	float ray_step;
	float brightness;

	bool jittering;
	bool transferF;
	bool clipping;
	bool isosurface;
	Texture* tf_texture;

	float threshold;
	Vector4 plane;

	vec3 light_pos;
	vec3 ambient_light;
	vec3 diffuse_light;
	vec3 specular_light;
	vec3 ka;
	vec3 kd;
	float alpha;
};

#endif