#pragma once
#include <vector>
#include "math/math.h"
#include "material.h"
#include <functional>

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

	Material* get_material() const { return _material; }
	void set_material(Material* material) { assert(material); _material = material; }

	typedef std::function<void(Mesh&, const Matrix4&)> DrawHandler;
	DrawHandler* get_pre_draw_handler() const { return _pre_draw_handler; }
	void set_pre_draw_handler(DrawHandler* handler) { _pre_draw_handler = handler; }
	DrawHandler* get_post_draw_handler() const { return _post_draw_handler; }
	void set_post_draw_handler(DrawHandler* handler) { _post_draw_handler = handler; }

private:
	void setup(const void* vertices_data);

	unsigned int _vao{ 0 };
	unsigned int _vbo{ 0 };
	unsigned int _ebo{ 0 };
	VertexFormat _vertex_format{ };
	unsigned int _vertices_count{ 0 };
	std::vector<unsigned int> _indices{ };
	Material* _material{ nullptr };

	DrawHandler* _pre_draw_handler{ nullptr };
	DrawHandler* _post_draw_handler{ nullptr };
};
