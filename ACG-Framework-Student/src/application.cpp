#include "application.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "volume.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "extra/hdre.h"
#include "extra/imgui/imgui.h"
#include "extra/imgui/imgui_impl_sdl.h"
#include "extra/imgui/imgui_impl_opengl3.h"

#include <cmath>

bool render_wireframe = false;
Camera* Application::camera = nullptr;
Application* Application::instance = NULL;

Application::Application(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;
	render_debug = true;
	autorotate = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	// OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	// Create camera
	camera = new Camera();
	camera->lookAt(Vector3(5.f, 5.f, 5.f), Vector3(0.f, 0.0f, 0.f), Vector3(0.f, 1.f, 0.f));
	camera->setPerspective(45.f, window_width/(float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	{
		// EXAMPLE OF HOW TO CREATE A SCENE NODE
		//SceneNode* node = new SceneNode("Visible node");
		//node->mesh = Mesh::Get("data/meshes/sphere.obj.mbin");
		//node->model.scale(1, 1, 1);
		//StandardMaterial* mat = new StandardMaterial();
		//node->material = mat;
		//mat->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/normal.fs");
		//node_list.push_back(node);

		// TODO: create all the volumes to use in the app

		////BONSAI
		{
			Volume* bonsai_volume = new Volume();
			Texture* bonsai_vol_texture = new Texture();
			Texture* bonsai_tf_texture = Texture::Get("data/images/tf_imagen.png");
			Mesh* bonsai_mesh = new Mesh();
			VolumeMaterial* bonsai_vol_mat = new VolumeMaterial();
			VolumeNode* bonsai_node = new VolumeNode("Volume node");

			bonsai_volume->loadPNG("data/volumes/bonsai_16_16.png");
			bonsai_vol_texture->create3DFromVolume(bonsai_volume, GL_CLAMP_TO_EDGE);
			bonsai_vol_mat->texture = bonsai_vol_texture;
			bonsai_vol_mat->tf_texture = bonsai_tf_texture;
			bonsai_vol_mat->plane = Vector4(15, 15, 20, -7);
			bonsai_node->material = bonsai_vol_mat;
			bonsai_node->mesh = bonsai_mesh;
			bonsai_node->mesh->createCube();

			float width_div = bonsai_volume->width * bonsai_volume->widthSpacing; //for right proportions
			bonsai_node->model.setScale(1, bonsai_volume->height * bonsai_volume->heightSpacing / width_div, bonsai_volume->depth * bonsai_volume->depthSpacing / width_div);
		
			node_list.push_back(bonsai_node);
		}

		////ABDOMEN
		{
			Volume* abdomen_volume = new Volume();
			Texture* abdomen_vol_texture = new Texture();
			Texture* abdomen_tf_texture = Texture::Get("data/images/tf_imagen.png");
			Mesh* abdomen_mesh = new Mesh();
			VolumeMaterial* abdomen_vol_mat = new VolumeMaterial();
			VolumeNode* abdomen_node = new VolumeNode("Volume node");

			abdomen_volume->loadPVM("data/volumes/CT-Abdomen.pvm");
			abdomen_vol_texture->create3DFromVolume(abdomen_volume, GL_CLAMP_TO_EDGE);
			abdomen_vol_mat->texture = abdomen_vol_texture;
			abdomen_vol_mat->tf_texture = abdomen_tf_texture;
			abdomen_vol_mat->plane = Vector4(15, 15, 20, -7);
			abdomen_node->material = abdomen_vol_mat;
			abdomen_node->mesh = abdomen_mesh;
			abdomen_node->mesh->createCube();

			float width_div = abdomen_volume->width * abdomen_volume->widthSpacing; //for right proportions
			abdomen_node->model.setScale(1, abdomen_volume->height * abdomen_volume->heightSpacing / width_div, abdomen_volume->depth * abdomen_volume->depthSpacing / width_div);
		
			node_list.push_back(abdomen_node);
		}

		////DAISY
		{
			Volume* daisy_volume = new Volume();
			Texture* daisy_vol_texture = new Texture();
			Texture* daisy_tf_texture = Texture::Get("data/images/tf_imagen.png");
			Mesh* daisy_mesh = new Mesh();
			VolumeMaterial* daisy_vol_mat = new VolumeMaterial();
			VolumeNode* daisy_node = new VolumeNode("Volume node");
			
			daisy_volume->loadPVM("data/volumes/Daisy.pvm");
			daisy_vol_texture->create3DFromVolume(daisy_volume, GL_CLAMP_TO_EDGE);
			daisy_vol_mat->texture = daisy_vol_texture;
			daisy_vol_mat->tf_texture = daisy_tf_texture;
			daisy_vol_mat->plane = Vector4(15, 15, 20, -7);
			daisy_node->material = daisy_vol_mat;
			daisy_node->mesh = daisy_mesh;
			daisy_node->mesh->createCube();
			
			float width_div = daisy_volume->width * daisy_volume->widthSpacing; //for right proportions
			daisy_node->model.setScale(1, daisy_volume->height * daisy_volume->heightSpacing / width_div, daisy_volume->depth * daisy_volume->depthSpacing / width_div);
			
			node_list.push_back(daisy_node);
		}

		////FOOT
		{
			Volume* foot_volume = new Volume();
			Texture* foot_vol_texture = new Texture();
			Texture* foot_tf_texture = Texture::Get("data/images/tf_imagen.png");
			Mesh* foot_mesh = new Mesh();
			VolumeMaterial* foot_vol_mat = new VolumeMaterial();
			VolumeNode* foot_node = new VolumeNode("Volume node");

			foot_volume->loadPNG("data/volumes/foot_16_16.png");
			foot_vol_texture->create3DFromVolume(foot_volume, GL_CLAMP_TO_EDGE);
			foot_vol_mat->texture = foot_vol_texture;
			foot_vol_mat->tf_texture = foot_tf_texture;
			foot_vol_mat->plane = Vector4(15, 15, 20, -7);
			foot_node->material = foot_vol_mat;
			foot_node->mesh = foot_mesh;
			foot_node->mesh->createCube();

			float width_div = foot_volume->width * foot_volume->widthSpacing; //for right proportions
			foot_node->model.setScale(1, foot_volume->height * foot_volume->heightSpacing / width_div, foot_volume->depth * foot_volume->depthSpacing / width_div);
		
			node_list.push_back(foot_node);
		}

		////ORANGE
		{
			Volume* orange_volume = new Volume();
			Texture* orange_vol_texture = new Texture();
			Texture* orange_tf_texture = Texture::Get("data/images/tf_imagen.png");
			Mesh* orange_mesh = new Mesh();
			VolumeMaterial* orange_vol_mat = new VolumeMaterial();
			VolumeNode* orange_node = new VolumeNode("Volume node");
			
			orange_volume->loadPVM("data/volumes/Orange.pvm");
			orange_vol_texture->create3DFromVolume(orange_volume, GL_CLAMP_TO_EDGE);
			orange_vol_mat->texture = orange_vol_texture;
			orange_vol_mat->tf_texture = orange_tf_texture;
			orange_vol_mat->plane = Vector4(15, 15, 20, -7);
			orange_node->material = orange_vol_mat;
			orange_node->mesh = orange_mesh;
			orange_node->mesh->createCube();
			
			float width_div = orange_volume->width * orange_volume->widthSpacing; //for right proportions
			orange_node->model.setScale(1, orange_volume->height * orange_volume->heightSpacing / width_div, orange_volume->depth * orange_volume->depthSpacing / width_div);
			
			node_list.push_back(orange_node);
		}

		////TEAPOT
		{
			Volume* teapot_volume = new Volume();
			Texture* teapot_vol_texture = new Texture();
			Texture* teapot_tf_texture = Texture::Get("data/images/tf_imagen.png");
			Mesh* teapot_mesh = new Mesh();
			VolumeMaterial* teapot_vol_mat = new VolumeMaterial();
			VolumeNode* teapot_node = new VolumeNode("Volume node");

			teapot_volume->loadPNG("data/volumes/teapot_16_16.png");
			teapot_vol_texture->create3DFromVolume(teapot_volume, GL_CLAMP_TO_EDGE);
			teapot_vol_mat->texture = teapot_vol_texture;
			teapot_vol_mat->tf_texture = teapot_tf_texture;
			teapot_vol_mat->plane = Vector4(15, 15, 20, -7);
			teapot_node->material = teapot_vol_mat;
			teapot_node->mesh = teapot_mesh;
			teapot_node->mesh->createCube();

			float width_div = teapot_volume->width * teapot_volume->widthSpacing; //for right proportions
			teapot_node->model.setScale(1, teapot_volume->height * teapot_volume->heightSpacing / width_div, teapot_volume->depth * teapot_volume->depthSpacing / width_div);
		
			node_list.push_back(teapot_node);
		}

	}
	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

// what to do when the image has to be draw
void Application::render(void)
{
	// set the clear color (the background color)
	glClearColor(0.1, 0.1, 0.1, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera as default
	camera->enable();

	// set flags
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	node_list[vol_index]->render(camera);

	if (render_wireframe)
		node_list[vol_index]->renderWireframe(camera);

	//for (size_t i = 0; i < node_list.size(); i++) {
	//
	//	node_list[i]->render(camera);
	//
	//	if(render_wireframe)
	//		node_list[i]->renderWireframe(camera);
	//}

	//Draw the floor grid
	if(render_debug) drawGrid();
}

void Application::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * 10; //the speed is defined by the seconds_elapsed so it goes constant
	float orbit_speed = seconds_elapsed * 1;

	// example
	float angle = seconds_elapsed * 10.f * DEG2RAD;
	if (autorotate)
	{
		node_list[vol_index]->model.rotate(angle, Vector3(0, 1, 0));

		//for (int i = 0; i < node_list.size(); i++) {
		//	node_list[i]->model.rotate(angle, Vector3(0, 1, 0));
		//}
	}

	// mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT && !ImGui::IsAnyWindowHovered()
		&& !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemActive())) //is left button pressed?
	{
		camera->orbit(-Input::mouse_delta.x * orbit_speed, Input::mouse_delta.y * orbit_speed);
	}

	// async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_Q) || Input::isKeyPressed(SDL_SCANCODE_SPACE)) camera->moveGlobal(Vector3(0.0f, -1.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_E) || Input::isKeyPressed(SDL_SCANCODE_LCTRL)) camera->moveGlobal(Vector3(0.0f, 1.0f, 0.0f) * speed);

	// to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

// Keyboard event handler (sync input)
void Application::onKeyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: render_debug = !render_debug; break;
		case SDLK_F2: render_wireframe = !render_wireframe; break;
		case SDLK_F5: Shader::ReloadAll(); break;
	}
}

void Application::onKeyUp(SDL_KeyboardEvent event)
{
}

void Application::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Application::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Application::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Application::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Application::onMouseWheel(SDL_MouseWheelEvent event)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
	{
		if (event.x > 0) io.MouseWheelH += 1;
		if (event.x < 0) io.MouseWheelH -= 1;
		if (event.y > 0) io.MouseWheel += 1;
		if (event.y < 0) io.MouseWheel -= 1;
	}
	}

	if (!ImGui::IsAnyWindowHovered() && event.y)
		camera->changeDistance(event.y * 0.5);
}

void Application::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	camera->aspect = width / (float)height;
	window_width = width;
	window_height = height;
}

void Application::onFileChanged(const char* filename)
{
	Shader::ReloadAll();
}

void Application::renderInMenu() {

	if (ImGui::TreeNode("Scene")) {
		//
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Camera")) {
		camera->renderInMenu();
		ImGui::TreePop();
	}

	//Scene graph
	if (ImGui::TreeNode("Entities"))
	{
		unsigned int count = 0;
		std::stringstream ss;
		//for (auto& node : node_list)
		{
			ss << count;
			if (ImGui::TreeNode(node_list[vol_index]->name.c_str()))
			{
				node_list[vol_index]->renderInMenu();
				ImGui::TreePop();
			}
			++count;
			ss.str("");
		}
		ImGui::TreePop();
	}

	//to pass to next node
	ImGui::Text("Select volume: %d", vol_index + 1);

	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{
		// Decrease the counter when left arrow is clicked
		if (vol_index == 0) vol_index = node_list.size();
		vol_index--;
	}

	ImGui::SameLine(); // Put the next UI element on the same line

	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{
		// Increase the counter when right arrow is clicked
		if (vol_index == node_list.size() - 1) vol_index = -1;
		vol_index++;
	}

	ImGui::Checkbox("Render debug", &render_debug);
	ImGui::Checkbox("Wireframe", &render_wireframe);
}
