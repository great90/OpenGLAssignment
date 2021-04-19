#include "mesh.h"

#include <utility>
#include "renderer.h"
#include "glad/glad.h"
#include "engine/engine.h"
#include "texture.h"
#include <iostream>
#include "graphic_api.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material)
{
	_vertices = std::move(vertices);
	_indices = std::move(indices);
	_material = material;

	setup();
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
		CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, _vertices.size()));
	}
	CHECK_GL_ERROR(glBindVertexArray(0));

	_material->deactive();
}

void Mesh::setup()
{
	CHECK_GL_ERROR(glGenVertexArrays(1, &_vao));
	CHECK_GL_ERROR(glGenBuffers(1, &_vbo));

	CHECK_GL_ERROR(glBindVertexArray(_vao));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW));

	if (!_indices.empty())
	{
		CHECK_GL_ERROR(glGenBuffers(1, &_ebo));
		CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo));
		CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW));
	}

	CHECK_GL_ERROR(glEnableVertexAttribArray(0));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	
	CHECK_GL_ERROR(glEnableVertexAttribArray(1));
	CHECK_GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

	CHECK_GL_ERROR(glEnableVertexAttribArray(2));
	CHECK_GL_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)));

	CHECK_GL_ERROR(glEnableVertexAttribArray(3));
	CHECK_GL_ERROR(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent)));

	CHECK_GL_ERROR(glEnableVertexAttribArray(4));
	CHECK_GL_ERROR(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent)));

	CHECK_GL_ERROR(glBindVertexArray(0));
}
