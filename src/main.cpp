#include <iostream>
#include "engine/engine.h"
#include "render/renderer.h"
#include "render/shader.h"
#include "render/render_object.h"
#include "render/texture.h"

RenderObject* init_light()
{
	static ShaderProgram shader("src/shader/light_vertex.shader", "src/shader/light_fragment.shader");
	if (!shader.valid())
	{
		std::cout << "Create shader program failed:" << shader.get_error_log() << std::endl;
		return nullptr;
	}
	//     7-------6
	//    /|      /|
	//   / |     / |
	//  4--+----5  |
	//  |  3----+--2
	//  | /     | /
	//  |/      |/
	//  0-------1
	float vertices[] =
	{
		// x      y      z
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	unsigned int indices[] = {
		0, 1, 5, 5, 4, 0, // front
		2, 3, 7, 7, 6, 2, // back
		0, 4, 7, 7, 3, 0, // left
		1, 2, 6, 6, 5, 1, // right
		4, 5, 6, 6, 7, 4, // top
		0, 3, 2, 2, 1, 0  // bottom
	};
	RenderObject::VertexFormat vf;
	vf.push_back({ 3, RenderObject::VertexAttr::ElementType::Float, false });

	RenderObject* light = Renderer::get_singleton().add_renderable(vf, vertices, 8, indices, sizeof(indices) / sizeof(indices[0]));
	light->set_position(Vector3(6.0f, 8.0f, -13.0f));
	light->set_shader(&shader);

	return light;
}

bool init_boxes(RenderObject* light)
{
	static Texture diffuse_texture;
	if (!diffuse_texture.load("asset/container2.png"))
	{
		return false;
	}
	static Texture specular_texture;
	if (!specular_texture.load("asset/container2_specular.png"))
	{
		return false;
	}

	static ShaderProgram shader("src/shader/box_vertex.shader", "src/shader/box_fragment.shader");
	if (!shader.valid())
	{
		std::cout << "Create shader program failed:" << shader.get_error_log() << std::endl;
		return false;
	}

	float vertices[] =
	{
		// x      y      z     nx     ny     nz     u     v
		// back
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		// front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		// left
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		// right
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		// bottom
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		// top
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	};
	unsigned int indices[] = {
		 0,  1,  2,  2,  3,  0,
		 4,  5,  6,  6,  7,  4,
		 8,  9, 10, 10, 11,  8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};
	Vector3 positions[] = {
		Vector3(0.0f,  0.0f,  0.0f),
		/*Vector3( 2.0f,  5.0f, -9.0f),
		Vector3(-1.5f, -2.2f, -2.5f),
		Vector3(-3.8f, -2.0f, -9.3f),
		Vector3( 2.4f, -0.4f, -3.5f),
		Vector3(-1.7f,  3.0f, -7.5f),
		Vector3( 1.3f, -2.0f, -2.5f),
		Vector3( 1.5f,  2.0f, -2.5f),
		Vector3( 1.5f,  0.2f, -1.5f),
		Vector3(-1.3f,  1.0f, -1.5f)*/
	};

	RenderObject::VertexFormat vf;
	vf.push_back({ 3, RenderObject::VertexAttr::ElementType::Float, false });
	vf.push_back({ 3, RenderObject::VertexAttr::ElementType::Float, false });
	vf.push_back({ 2, RenderObject::VertexAttr::ElementType::Float, false });

	for (auto box_position : positions)
	{
		RenderObject* object = Renderer::get_singleton().add_renderable(vf, vertices, 24, indices, sizeof(indices) / sizeof(indices[0]));
		object->set_position(box_position);
		object->set_rotation(Vector3(20.0f, -20.0f, -10.0f));
		object->set_diffuse_texture(&diffuse_texture);
		object->set_specular_texture(&specular_texture);
		object->set_shader(&shader);

		object->set_light(light);
	}

	return true;
}

int main()
{
	std::shared_ptr<Engine> engine = std::make_shared<Engine>();
	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();

	if (!engine->startup())
		return -1;

	RenderObject* light = init_light();
	if (!light || !init_boxes(light))
		return -1;

	engine->run();
	engine->shutdown();
	
    return 0;
}