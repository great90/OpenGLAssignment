#pragma once

#include "common/singleton.h"
#include "math/math.h"

struct GLFWwindow;
class ShaderProgram;
class Texture;
class RenderObject;
class Camera;

class Engine : public Singleton<Engine>
{
public:
	Engine() = default;
	~Engine() = default;

	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) = delete;

	bool startup();
	void shutdown();
	void run();

	void set_camera(Camera* camera) { _camera = camera; }
	Camera* get_camera() const { return _camera; }

	void set_should_shutdown() { _should_shutdown = true; }

	float get_time() const;

private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_move_callback(GLFWwindow* window, double x, double y);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void on_framebuffer_sized(int width, int height);
	void on_mouse_moved(Vector2 position);
	void on_mouse_scrolled(float offset);
	void process_input(float delta);

	struct GLFWwindow* _window = nullptr;
	Camera* _camera = nullptr;
	float _last_frame_time = 0.0f;
	bool _should_shutdown = false;

	bool _mouse_moved = false;
	Vector2 _last_mouse_position{0.0f, 0.0f};
};
