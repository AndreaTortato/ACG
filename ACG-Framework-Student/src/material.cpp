#include "material.h"
#include "texture.h"
#include "application.h"
#include "extra/hdre.h"

StandardMaterial::StandardMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}

StandardMaterial::~StandardMaterial()
{

}

void StandardMaterial::setUniforms(Camera* camera, Matrix44 model)
{
	//upload node uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Application::instance->time);
	shader->setUniform("u_color", color);

	if (texture)
		shader->setUniform("u_texture", texture);
}

void StandardMaterial::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	if (mesh && shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}
}

void StandardMaterial::renderInMenu()
{
	ImGui::ColorEdit3("Color", (float*)&color); // Edit 3 floats representing a color
}

WireframeMaterial::WireframeMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}

WireframeMaterial::~WireframeMaterial()
{

}

void WireframeMaterial::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	if (shader && mesh)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//enable shader
		shader->enable();

		//upload material specific uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


VolumeMaterial::VolumeMaterial()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/volumetric.fs");
	ray_step = 0.02;
	brightness = 2.0;

	jittering = false;
	transferF = false;
	clipping = true;
	isosurface = false;

	threshold = 0.5;

	light_pos = vec3(10.0, 10.0, 10.0);
	ambient_light = vec3(5.0, 5.0, 5.0);
	diffuse_light = vec3(3.0, 3.0, 4.0);
	specular_light = vec3(1.0, 1.0, 1.0);;
	ka = vec3(2.0, 1.0, 1.0);
	kd = vec3(1.0, 2.0, 1.0);
	alpha = 20.0;
}

VolumeMaterial::~VolumeMaterial()
{

}

void VolumeMaterial::setUniforms(Camera* camera, Matrix44 model)
{
	
	Matrix44 invModel = model;
	invModel.inverse();
	Vector3 localCamPos = invModel * camera->eye;  
	

	//upload node uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_pos", camera->eye);
	shader->setUniform("u_local_camera_pos", localCamPos);
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Application::instance->time);
	shader->setUniform("u_color", color);
	shader->setUniform("u_ray_step_lenght", ray_step);
	shader->setUniform("u_brightness", brightness);

	shader->setUniform("u_jittering", jittering);
	shader->setUniform("u_transferF", transferF);
	shader->setUniform("u_clipping", clipping);
	shader->setUniform("u_isosurface", isosurface);
	shader->setUniform("u_tf_texture", tf_texture);

	shader->setUniform("u_plane", plane);
	shader->setUniform("u_threshold", threshold);

	shader->setUniform("light_pos", light_pos);
	shader->setUniform("ambient_light", ambient_light);
	shader->setUniform("diffuse_light", diffuse_light);
	shader->setUniform("specular_light", specular_light);
	shader->setUniform("ka", ka);
	shader->setUniform("kd", kd);
	shader->setUniform("alpha", alpha);

	if (texture) shader->setTexture("u_volume_texture", texture, 0);
}

void VolumeMaterial::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	if (mesh && shader)
	{
		glEnable(GL_BLEND);

		//enable shader
		shader->enable();

		//upload uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();

		glDisable(GL_BLEND);
	}
}

void VolumeMaterial::renderInMenu()
{
	ImGui::ColorEdit3("Color", (float*)&color); // Edit 3 floats representing a color
	ImGui::SliderFloat("Ray Step Length", (float*)&ray_step, 0.001, 0.3);
	ImGui::SliderFloat("Brightness", (float*)&brightness, 0.1, 10.0);
	//ImGui::SliderFloat("TF threshold", (float*)&threshold, 0.0, 1.0);

	ImGui::Checkbox("Jittering", (bool*)&jittering);
	ImGui::Checkbox("Transfer Function", (bool*)&transferF);
	ImGui::Checkbox("Clipping", (bool*)&clipping);
	ImGui::Checkbox("Isosurface", (bool*)&isosurface);

}