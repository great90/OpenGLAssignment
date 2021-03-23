#pragma once

#include "common/singleton.h"
#include <vector>

struct GLFWwindow;
class ShaderProgram;
class RenderObject;

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

	void set_should_shutdown() { _should_shutdown = true; }
	
private:
	void process_input();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	
	struct GLFWwindow* _window = nullptr;
	ShaderProgram* _shader = nullptr;
	std::vector<RenderObject*> _render_objects;
	bool _should_shutdown = false;
};
