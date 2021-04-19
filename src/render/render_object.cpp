#include "render_object.h"
#include "glad/glad.h"
#include <cassert>
#include "engine/engine.h"
#include "texture.h"
#include "shader.h"
#include "engine/camera.h"
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/quaternion.hpp>
#include "renderer.h"
#include "graphic_api.h"

RenderObject::RenderObject(const VertexFormat& vertex_format, const void* vertex_data, size_t vertex_count, const unsigned* indices, size_t index_count)
{
	std::vector<unsigned> offsets;
	unsigned int vertex_size = 0;
	for (const auto& attr : vertex_format)
	{
		offsets.emplace_back(vertex_size);
		size_t element_size = sizeof(float);
		switch (attr.element_type)
		{
		case VertexAttr::ElementType::Int:
			element_size = sizeof(int);
			break;
		default:
			break;
		}
		vertex_size += element_size * attr.element_count;
	}
	assert(vertex_count >= 3);
	CHECK_GL_ERROR(glGenVertexArrays(1, &_vao));
	CHECK_GL_ERROR(glGenBuffers(1, &_vbo));
	_vertex_count = vertex_count;
	_index_count = 0;
	
	CHECK_GL_ERROR(glBindVertexArray(_vao));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, vertex_size * vertex_count, vertex_data, GL_STATIC_DRAW));

	assert(index_count % 3 == 0);
	if (indices != nullptr && index_count > 0)
	{
		_index_count = index_count;
		CHECK_GL_ERROR(glGenBuffers(1, &_ebo));
		CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
		CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * index_count, indices, GL_STATIC_DRAW));
	}

	for (size_t i = 0; i < vertex_format.size(); ++i)
	{
		const auto& attr = vertex_format[i];
		auto element_type = GL_FLOAT;
		switch (attr.element_type)
		{
		case VertexAttr::ElementType::Int:
			element_type = GL_INT;
			break;
		default:
			break;
		}
		CHECK_GL_ERROR(glVertexAttribPointer(i, attr.element_count, element_type, attr.normalization, vertex_size, (void*)(offsets[i])));
		CHECK_GL_ERROR(glEnableVertexAttribArray(i));
	}

	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CHECK_GL_ERROR(glBindVertexArray(0));
}

RenderObject::~RenderObject()
{
	CHECK_GL_ERROR(glDeleteVertexArrays(1, &_vao));
	CHECK_GL_ERROR(glDeleteBuffers(1, &_vbo));
	if (_index_count > 0)
	{
		CHECK_GL_ERROR(glDeleteBuffers(1, &_ebo));
	}
}

void RenderObject::render() const
{
	_shader->bind();
	Renderer::get_singleton().bind_shader_data(*_shader);

	if (_diffuse_texture)
	{
		_diffuse_texture->active(0);
		_shader->set_int("material.diffuse", 0);
	}	
	if (_specular_texture)	// TODO
	{
		_specular_texture->active(1);
		_shader->set_int("material.specular", 1);
		_shader->set_float("material.shininess", 64.0f);
	}

	auto model = get_model_matrix();
	//model = glm::rotate(model, glm::radians(engine.get_time() * 30), glm::vec3(1.0f, 0.3f, 0.5f));
	_shader->set_matrix4("model", model);
	
	CHECK_GL_ERROR(glBindVertexArray(_vao));
	if (_index_count > 0)
	{
		CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, 0));
	}
	else
	{
		CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, _vertex_count));
	}

	_shader->unbind();
}

Matrix4 RenderObject::get_model_matrix() const
{
	const Quaternion q = Quaternion(glm::radians(glm::vec3(_rotation.x, _rotation.y, _rotation.z)));
	Matrix4 model = glm::mat4(1.0f);
	model = glm::scale(model, _scale);
	model = glm::mat4_cast(q) * model;
	return glm::translate(model, _position);
}
