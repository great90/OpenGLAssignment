#include "render_object.h"
#include "glad/glad.h"
#include <cassert>
#include "engine/engine.h"
#include "texture.h"
#include "shader.h"
#include "engine/camera.h"
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/quaternion.hpp>

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
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	_vertex_count = vertex_count;
	_index_count = 0;
	
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_size * vertex_count, vertex_data, GL_STATIC_DRAW);

	assert(index_count % 3 == 0);
	if (indices != nullptr && index_count > 0)
	{
		_index_count = index_count;
		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * index_count, indices, GL_STATIC_DRAW);
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
		glVertexAttribPointer(i, attr.element_count, element_type, attr.normalization, vertex_size, (void*)(offsets[i]));
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

RenderObject::~RenderObject()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	if (_index_count > 0)
	{
		glDeleteBuffers(1, &_ebo);
	}
}

void RenderObject::render() const
{
	Engine& engine = Engine::get_singleton();
	Camera* camera = engine.get_camera();

	if (_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		_texture->active();
	}
	
	_shader->bind();
	if (_texture)	// TODO
	{
		_shader->set_int("tex", 0);
		_shader->set_vector3("objectColor", 1.0f, 0.5f, 0.31f);
	}

	if (_light)
	{
		_shader->set_vector3("lightColor", 1.0f, 1.0f, 1.0f);
		_shader->set_vector3("lightPos", _light->get_position());
		_shader->set_vector3("viewPos", Engine::get_singleton().get_camera()->get_position());
	}
	_shader->set_matrix4("projection", camera->get_projection_matrix());
	_shader->set_matrix4("view", camera->get_view_matrix());
	auto model = get_model_matrix();
	//model = glm::rotate(model, glm::radians(engine.get_time() * 30), glm::vec3(1.0f, 0.3f, 0.5f));
	_shader->set_matrix4("model", model);
	
	glBindVertexArray(_vao);
	if (_index_count > 0)
	{
		glDrawElements(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, _vertex_count);
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
