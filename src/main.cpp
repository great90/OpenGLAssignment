#include <iostream>
#include "engine/engine.h"
#include "render/renderer.h"
#include "render/shader.h"
#include "render/render_object.h"
#include "render/texture.h"

int main()
{
	std::shared_ptr<Engine> engine = std::make_shared<Engine>();
	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();

	if (!engine->startup())
		return -1;

	Texture texture;
	if (!texture.load("asset/container.jpg"))
	{
		return -1;
	}

	ShaderProgram shader("src/shader/vertex.shader", "src/shader/fragment.shader");
	if (!shader.valid())
	{
		std::cout << "Create shader program failed:" << shader.get_error_log() << std::endl;
		return -1;
	}

	float vertices[] =
	{
		// x      y      z     r     g     b	 u     v
		// back
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		// front
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		// left
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		// right
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		// bottom
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		// top
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
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
		Vector3( 0.0f,  0.0f,  0.0f),
		Vector3( 2.0f,  5.0f, -9.0f),
		Vector3(-1.5f, -2.2f, -2.5f),
		Vector3(-3.8f, -2.0f, -9.3f),
		Vector3( 2.4f, -0.4f, -3.5f),
		Vector3(-1.7f,  3.0f, -7.5f),
		Vector3( 1.3f, -2.0f, -2.5f),
		Vector3( 1.5f,  2.0f, -2.5f),
		Vector3( 1.5f,  0.2f, -1.5f),
		Vector3(-1.3f,  1.0f, -1.5f)
	};
	
	RenderObject::VertexFormat vf;
	vf.push_back({ 3, RenderObject::VertexAttr::ElementType::Float, false });
	vf.push_back({ 3, RenderObject::VertexAttr::ElementType::Float, false });
	vf.push_back({ 2, RenderObject::VertexAttr::ElementType::Float, false });

	for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); ++i)
	{
		RenderObject* object = renderer->add_renderable(vf, vertices, 24, indices, sizeof(indices) / sizeof(indices[0]));
		object->set_position(positions[i]);
		object->set_texture(&texture);
		object->set_shader(&shader);
	}
	engine->run();
	engine->shutdown();
	
    return 0;
}