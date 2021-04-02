#include "renderer.h"
#include <glad/glad.h>

Renderer* Singleton<Renderer>::singleton = nullptr;

Renderer::Renderer()
	: _clear_color(0.2f, 0.3f, 0.3f, 1.0f)
{
	
}

Renderer::~Renderer()
{
	for (auto object : _render_objects)
	{
		delete object;
	}
}

void Renderer::begin_frame(float delta)
{
	glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::end_frame(bool swap_buffer)
{
	// TODO swap buffer
}

void Renderer::draw(float delta)
{
	begin_frame(delta);
	
	for (auto object : _render_objects)
	{
		object->render();
	}

	end_frame(true);
}

RenderObject* Renderer::add_renderable(const RenderObject::VertexFormat& vformat, const void* vertices, size_t vcount, const unsigned int* indices/*=nullptr*/, size_t icount/*=0*/)
{
	RenderObject* object = new RenderObject(vformat, vertices, vcount, indices, icount);
	_render_objects.emplace_back(object);
	return object;
}
