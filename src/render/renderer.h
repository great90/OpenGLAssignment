#pragma once
#include "color.h"
#include "common/singleton.h"
#include <vector>
#include "render_object.h"
#include "shader.h"
#include "Light.h"

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

	Light& get_directional_light() { return _directional_light; }
	//void set_directional_light(const Light& light) { assert(light.type == LightType::Directional); _directional_light = light; }
	void add_omni_light(Light light) { assert(light.type == LightType::Omni); _omni_lights.push_back(light); }
	const std::vector<Light>& get_omni_lights() const { return _omni_lights; }
	void add_spot_light(Light light) { assert(light.type == LightType::Spot); _spot_lights.push_back(light); }
	const std::vector<Light>& get_spot_lights() const { return _spot_lights; }

protected:
	void begin_frame(float delta);
	void end_frame(bool swap_buffer);

private:
	Color _clear_color;	
	std::vector<RenderObject*> _render_objects;

	Light _directional_light{ };
	std::vector<Light> _omni_lights{ };
	std::vector<Light> _spot_lights{ };
};
