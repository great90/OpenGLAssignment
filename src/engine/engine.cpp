#include "engine.h"
#include <iostream>
#include "render/renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "render/shader.h"
#include "render/render_object.h"

Engine* Singleton<Engine>::singleton = nullptr;

const char *vertex_shader_source =
	"#version 330 core\n"
	"layout (location = 0) in vec3 vPos;\n"
	"layout (location = 1) in vec3 vColor;\n"
	"out vec3 fColor;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);\n"
	"   fColor = vColor;\n"
	"}\0";

const char *fragment_shader_source =
	"#version 330 core\n"
	"uniform float time;"
	"in vec3 fColor;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    vec3 green = vec3(0.5f, (sin(time) / 2.0f + 1.0f), 0.5f);\n"
	"   FragColor = vec4(fColor * green, 1.0f);\n"
	"}\n\0";


bool Engine::startup()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	ShaderObject vs(ShaderObject::Type::Vertex, vertex_shader_source);
	if (!vs.valid())
	{
		std::cout << "Create vertex shader failed:" << vs.get_compile_log() << std::endl;
		glfwTerminate();
		return false;
	}
	ShaderObject fs(ShaderObject::Type::Fragment, fragment_shader_source);
	if (!fs.valid())
	{
		std::cout << "Create fragment shader failed:" << fs.get_compile_log() << std::endl;
		glfwTerminate();
		return false;
	}
	std::vector<const ShaderObject*> shaders;
	shaders.resize(2);
	shaders[(int)ShaderObject::Type::Vertex] = &vs;
	shaders[(int)ShaderObject::Type::Fragment] = &fs;
	_shader = new ShaderProgram(shaders);
	if (!_shader->valid())
	{
		std::cout << "Create shader program failed:" << _shader->get_link_log() << std::endl;
		delete _shader;
		glfwTerminate();
		return false;
	}

	/*
	float vertices[] =
	{
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	*/
	float vertices[] =
	{
		// x      y     z     r     g     b
		-0.4f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top
		-0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // left1
		 0.1f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // right1
		-0.1f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // left2
		 0.9f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // right2
		 0.4f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // bottom
	};
	unsigned int indices[] =
	{
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	RenderObject::VertexFormat vf;
	vf.push_back({ 3, RenderObject::VertexAttr::ElementType::Float, false });
	vf.push_back({ 3, RenderObject::VertexAttr::ElementType::Float, false });
	RenderObject* ro = new RenderObject(vf, vertices, 6, nullptr, 0);
	_render_objects.push_back(ro);
	
	return true;
}

void Engine::shutdown()
{
	delete _shader;
	for (auto ro : _render_objects)
	{
		delete ro;
	}
	glfwTerminate();
}

void Engine::run()
{
	Renderer& renderer = Renderer::get_singleton();
	
	while (!_should_shutdown && !glfwWindowShouldClose(_window))
	{
		process_input();

		renderer.begin_frame(0.0f);

		_shader->bind();
		_shader->set_float("time", (float)glfwGetTime());
		for (auto ro : _render_objects)
		{
			ro->render();
		}
		_shader->unbind();
		
		renderer.end_frame(true);
		
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Engine::process_input()
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_window, true);
	}
}