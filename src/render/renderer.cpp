#include "renderer.h"
#include <glad/glad.h>

Renderer* Singleton<Renderer>::singleton = nullptr;

Renderer::Renderer()
	: _clear_color(0.2f, 0.3f, 0.3f, 1.0f)
{
	
}

Renderer::~Renderer()
{
	
}

void Renderer::begin_frame(float step)
{
	glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::end_frame(bool swap_buffer)
{
	
}
