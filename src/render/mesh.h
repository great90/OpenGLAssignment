#pragma once
#include <vector>
#include "math/math.h"
#include "material.h"

class ShaderProgram;
class Texture;

class Mesh
{
public:
	struct VertexAttr
	{
		enum class ElementType : int
		{
			Float = 0,
			Int
		};
		size_t element_count;
		ElementType element_type;
		bool normalization;
	};
	typedef std::vector<VertexAttr> VertexFormat;
	
	Mesh(VertexFormat vertex_format, const void* vertices_data, size_t vertices_count, std::vector<unsigned int> indices, Material* material);

	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;

	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&&) = delete;
	
	~Mesh();

	void draw(const Matrix4& model) const;

private:
	void setup(const void* vertices_data);

	unsigned int _vao{ 0 };
	unsigned int _vbo{ 0 };
	unsigned int _ebo{ 0 };
	VertexFormat _vertex_format{ };
	unsigned int _vertices_count{ 0 };
	std::vector<unsigned int> _indices;
	Material* _material;
};
