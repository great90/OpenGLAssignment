#pragma once
#include "color.h"
#include "common/singleton.h"
#include <vector>
#include "render_object.h"
#include "shader.h"

class Renderer : public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	void draw(float delta);
	void set_clear_color(Color color) { _clear_color = color; }
	Color get_clear_color() const { return _clear_color; }
	
	RenderObject* add_renderable(const RenderObject::VertexFormat& vformat, const void* vertices, size_t vcount, const unsigned int* indices = nullptr, size_t icount = 0);

protected:
	void begin_frame(float delta);
	void end_frame(bool swap_buffer);

private:
	Color _clear_color;	
	std::vector<RenderObject*> _render_objects;
};
