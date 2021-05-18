#include "renderer.h"
#include <glad/glad.h>
#include "engine/engine.h"
#include "engine/camera.h"
#include "model.h"
#include "material.h"
#include "graphic_api.h"

Renderer* Singleton<Renderer>::singleton = nullptr;

void Renderer::begin_frame(float delta)
{
	CHECK_GL_ERROR(glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a));
	CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	_render_list.clear();
}

void Renderer::end_frame(bool swap_buffer)
{
	std::vector<RenderInfo> opaque_list{ };
	std::vector<RenderInfo> translucent_list{ };

	for (const auto& info : _render_list)
	{
		if (info.first->get_material()->is_translucence())
			translucent_list.emplace_back(info);
		else
			opaque_list.emplace_back(info);
	}
	const auto camera_pos = Engine::get_singleton().get_camera()->get_position();
	std::sort(translucent_list.begin(), translucent_list.end(), [=](RenderInfo& lhs, RenderInfo& rhs)
	{
		const auto lpos = Vector3(lhs.second[3]);
		const auto rpos = Vector3(rhs.second[3]);
		const auto llen = distance_sqr(camera_pos, lpos);
		const auto rlen = distance_sqr(camera_pos, rpos);
		return llen > rlen;
	});
	draw_render_list(opaque_list);
	draw_render_list(translucent_list);

	// TODO swap buffer
}

void Renderer::draw_render_list(const std::vector<RenderInfo>& render_list)
{
	for (const auto& info : render_list)
	{
		auto* mesh = info.first;
		auto model = info.second;
		if (const auto handler = mesh->get_pre_draw_handler())
		{
			(*handler)(*mesh, model);
		}
		mesh->draw(model);
		if (const auto handler = mesh->get_post_draw_handler())
		{
			(*handler)(*mesh, model);
		}
	}
}

void Renderer::draw(float delta)
{
	begin_frame(delta);

	const auto* camera = Engine::get_singleton().get_camera();
	_spot_lights[0].spot.position = camera->get_position();
	_spot_lights[0].spot.direction = camera->get_forward();

	for (auto model : _models)
	{
		model->draw(_render_list);
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
