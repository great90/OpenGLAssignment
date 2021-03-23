#pragma once
#include "color.h"
#include "common/singleton.h"

class Renderer : public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	void begin_frame(float step);
	void end_frame(bool swap_buffer);

	void set_clear_color(Color color) { _clear_color = color; }
	Color get_clear_color() const { return _clear_color; }
	
private:
	Color _clear_color;	
};
