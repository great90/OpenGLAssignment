#include "engine.h"
#include <iostream>
#include "render/renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"

Engine* Singleton<Engine>::singleton = nullptr;

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

	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);
	const Vector3 pos(0.0f, 0.0f, 8.0f);
	const Vector3 forward(0.0f, 0.0f, -1.0f);
	_camera = new Camera(45.0f, (float)width / (float)height, 0.5f, 100.0f, pos, forward);

	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(_window, mouse_move_callback);
	glfwSetScrollCallback(_window, mouse_scroll_callback);

	_last_frame_time = get_time();

	return true;
}

void Engine::shutdown()
{
	glfwTerminate();
}

void Engine::run()
{
	Renderer& renderer = Renderer::get_singleton();
	
	while (!_should_shutdown && !glfwWindowShouldClose(_window))
	{
		const float time = get_time();
		const float delta = time - _last_frame_time;
		_last_frame_time = time;

		process_input(delta);

		renderer.draw(delta);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

float Engine::get_time() const
{
	return glfwGetTime();
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Engine::get_singleton().on_framebuffer_sized(width, height);
}

void Engine::mouse_move_callback(GLFWwindow* window, double x, double y)
{
	Engine::get_singleton().on_mouse_moved(Vector2((float)x, (float)y));
}

void Engine::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Engine::get_singleton().on_mouse_scrolled((float)yoffset);
}

void Engine::on_framebuffer_sized(int width, int height)
{
	glViewport(0, 0, width, height);
	_camera->set_aspect((float)width / (float)height);
}

void Engine::on_mouse_moved(Vector2 position)
{
	if (!_mouse_moved)
	{
		_last_mouse_position = position;
		_mouse_moved = true;
		return;
	}
	const Vector2 offset = (position - _last_mouse_position) * 0.1f;
	_last_mouse_position = position;

	if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		_camera->rotate(offset.x, -offset.y); // reversed since y-coordinates go from bottom to top
	}
}

void Engine::on_mouse_scrolled(float offset)
{
	_camera->set_fov(_camera->get_fov() - offset);
}

void Engine::process_input(float delta)
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_window, true);
	}

	const float camera_speed = 0.05f; // adjust accordingly
	const Vector3& forward = _camera->get_forward(); // already normalized
	const Vector3& up = _camera->get_up();	// already normalized
	const Vector3 right = glm::normalize(glm::cross(forward, up));
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
		_camera->move(camera_speed * forward);
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
		_camera->move(-camera_speed * forward);
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
		_camera->move(-camera_speed * right);
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
		_camera->move(camera_speed * right);
	if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
		_camera->move(camera_speed * up);
	if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
		_camera->move(-camera_speed * up);
}