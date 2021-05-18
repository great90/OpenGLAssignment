#include "mesh.h"

#include <utility>
#include "renderer.h"
#include "glad/glad.h"
#include "engine/engine.h"
#include "texture.h"
#include "material.h"
#include "graphic_api.h"

Mesh::Mesh(VertexFormat vertex_format, const void* vertices_data, size_t vertices_count, std::vector<unsigned int> indices, Material* material)
	: _vertex_format(std::move(vertex_format))
	, _vertices_count(vertices_count)
	, _indices(std::move(indices))
	, _material(material)
{
	setup(vertices_data);
}

Mesh::~Mesh()
{
	CHECK_GL_ERROR(glDeleteVertexArrays(1, &_vao));
	CHECK_GL_ERROR(glDeleteBuffers(1, &_vbo));
	if (!_indices.empty())
	{
		CHECK_GL_ERROR(glDeleteBuffers(1, &_ebo));
	}
}

void Mesh::draw(const Matrix4& model) const
{
	_material->active(model);
	
	CHECK_GL_ERROR(glBindVertexArray(_vao));
	if (!_indices.empty())
	{
		CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0));
	}
	else
	{
		CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, _vertices_count));
	}
	CHECK_GL_ERROR(glBindVertexArray(0));

	_material->deactive();
}

void Mesh::setup(const void* vertices_data)
{
	assert(_vertices_count >= 3);
	assert(_indices.size() % 3 == 0);

	unsigned int vertex_size = 0;
	for (const auto& attr : _vertex_format)
	{
		switch (attr.element_type)
		{
		case VertexAttr::ElementType::Float: vertex_size += sizeof(float) * attr.element_count; break;
		case VertexAttr::ElementType::Int:   vertex_size += sizeof(int) * attr.element_count; break;
		default: assert(false);
		}
	}
	CHECK_GL_ERROR(glGenVertexArrays(1, &_vao));
	CHECK_GL_ERROR(glGenBuffers(1, &_vbo));

	CHECK_GL_ERROR(glBindVertexArray(_vao));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, vertex_size * _vertices_count, vertices_data, GL_STATIC_DRAW));

	if (!_indices.empty())
	{
		CHECK_GL_ERROR(glGenBuffers(1, &_ebo));
		CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
		CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW));
	}

	unsigned int offset = 0;
	for (size_t i = 0; i < _vertex_format.size(); ++i)
	{
		const auto& attr = _vertex_format[i];
		switch (attr.element_type)
		{
		case VertexAttr::ElementType::Float:
			CHECK_GL_ERROR(glVertexAttribPointer(i, attr.element_count, GL_FLOAT, attr.normalization, vertex_size, (void*)offset));
			offset += sizeof(float) * attr.element_count;
			break;
		case VertexAttr::ElementType::Int:
			CHECK_GL_ERROR(glVertexAttribPointer(i, attr.element_count, GL_INT, attr.normalization, vertex_size, (void*)offset));
			offset += sizeof(int) * attr.element_count;
			break;
		default: assert(false);
		}
		CHECK_GL_ERROR(glEnableVertexAttribArray(i));
	}

	CHECK_GL_ERROR(glBindVertexArray(0));
}
