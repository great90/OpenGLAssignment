#include <iostream>
#include "engine/engine.h"
#include "render/renderer.h"
#include "render/shader.h"
#include "render/render_object.h"
#include "render/texture.h"

const char *vertex_shader_source =
	"#version 330 core\n"
	"layout (location = 0) in vec3 vPos;\n"
	"layout (location = 1) in vec3 vColor;\n"
	"layout (location = 2) in vec2 vUV;\n"
	"uniform mat4 projection;"
	"uniform mat4 view;"
	"uniform mat4 model;"
	"out vec3 fColor;\n"
	"out vec2 fUV;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = projection * view * model * vec4(vPos, 1.0);\n"
	"   fColor = vColor;\n"
	"	fUV = vUV;\n"
	"}\0";

const char *fragment_shader_source =
	"#version 330 core\n"
	"uniform sampler2D tex;"
	"in vec3 fColor;\n"
	"in vec2 fUV;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    FragColor = texture(tex, fUV) * vec4(fColor, 1.0f);\n"
	"}\n\0";


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

	ShaderObject vs(ShaderObject::Type::Vertex, vertex_shader_source);
	if (!vs.valid())
	{
		std::cout << "Create vertex shader failed:" << vs.get_compile_log() << std::endl;
		return -1;
	}
	ShaderObject fs(ShaderObject::Type::Fragment, fragment_shader_source);
	if (!fs.valid())
	{
		std::cout << "Create fragment shader failed:" << fs.get_compile_log() << std::endl;
		return -1;
	}
	std::vector<const ShaderObject*> shaders;
	shaders.resize(2);
	shaders[(int)ShaderObject::Type::Vertex] = &vs;
	shaders[(int)ShaderObject::Type::Fragment] = &fs;
	ShaderProgram shader(shaders);
	if (!shader.valid())
	{
		std::cout << "Create shader program failed:" << shader.get_link_log() << std::endl;
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