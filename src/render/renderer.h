#pragma once
#include "color.h"
#include "common/singleton.h"
#include <vector>
#include "shader.h"
#include "light.h"
#include <set>

class Model;

class Renderer : public Singleton<Renderer>
{
public:
	Renderer() = default;
	~Renderer() { cleanup(); }

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	void draw(float delta);
	void set_clear_color(Color color) { _clear_color = color; }
	Color get_clear_color() const { return _clear_color; }

	void add_model(Model* model) { _models.emplace(model); }

	Light& get_directional_light() { return _directional_light; }
	//void set_directional_light(const Light& light) { assert(light.type == LightType::Directional); _directional_light = light; }
	void add_omni_light(Light light) { assert(light.type == LightType::Omni); _omni_lights.push_back(light); }
	const std::vector<Light>& get_omni_lights() const { return _omni_lights; }
	void add_spot_light(Light light) { assert(light.type == LightType::Spot); _spot_lights.push_back(light); }
	const std::vector<Light>& get_spot_lights() const { return _spot_lights; }

	void bind_shader_data(ShaderProgram& shader) const;

	void cleanup();
	
protected:
	void begin_frame(float delta);
	void end_frame(bool swap_buffer);

private:
	Color _clear_color{ 0.2f, 0.3f, 0.3f, 1.0f };
	std::set<Model*> _models{ };

	Light _directional_light{ };
	std::vector<Light> _omni_lights{ };
	std::vector<Light> _spot_lights{ };
};
