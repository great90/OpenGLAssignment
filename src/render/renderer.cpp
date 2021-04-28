#include "renderer.h"
#include <glad/glad.h>
#include "engine/engine.h"
#include "engine/camera.h"
#include "model.h"
#include "graphic_api.h"

Renderer* Singleton<Renderer>::singleton = nullptr;

void Renderer::begin_frame(float delta)
{
	CHECK_GL_ERROR(glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a));
	CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::end_frame(bool swap_buffer)
{
	// TODO swap buffer
}

void Renderer::draw(float delta)
{
	begin_frame(delta);

	const auto* camera = Engine::get_singleton().get_camera();
	_spot_lights[0].spot.position = camera->get_position();
	_spot_lights[0].spot.direction = camera->get_forward();

	for (auto model : _models)
	{
		model->draw();
	}

	end_frame(true);
}

void Renderer::bind_shader_data(ShaderProgram& shader) const
{
	const auto camera = Engine::get_singleton().get_camera();
	shader.set_vector3("viewPos", camera->get_position());
	shader.set_matrix4("projection", camera->get_projection_matrix());
	shader.set_matrix4("view", camera->get_view_matrix());
	shader.set_float("camera_near", camera->get_near());
	shader.set_float("camera_far", camera->get_far());

	_directional_light.bind(shader, "directional_light");

	for (size_t i = 0; i < _omni_lights.size(); ++i)
	{
		_omni_lights[i].bind(shader, "omni_lights[" + std::to_string(i) + "]");
	}
	shader.set_int("omni_light_count", (int)_omni_lights.size());

	for (size_t i = 0; i < _spot_lights.size(); ++i)
	{
		_spot_lights[i].bind(shader, "spot_lights[" + std::to_string(i) + "]");
	}
	shader.set_int("spot_light_count", (int)_spot_lights.size());
}

void Renderer::cleanup()
{
	for (auto model : _models)
	{
		delete model;
	}
	_models.clear();
}
