#pragma once
#include <vector>
#include "math/math.h"

class ShaderProgram;
class Texture;

class RenderObject
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
	
	RenderObject(const VertexFormat& vertex_format, const void* vertex_data, size_t vertex_count, const unsigned int* indices, size_t index_count);
	~RenderObject();

	void render() const;

	Matrix4 get_model_matrix() const;
	
	ShaderProgram* get_shader() const { return _shader; }
	void set_shader(ShaderProgram* shader) { _shader = shader; }
	Texture* get_diffuse_texture() const { return _diffuse_texture; }
	void set_diffuse_texture(Texture* texture) { _diffuse_texture = texture; }
	Texture* get_specular_texture() const { return _specular_texture; }
	void set_specular_texture(Texture* texture) { _specular_texture = texture; }
	const Vector3& get_position() const { return _position; }
	void set_position(const Vector3& position) { _position = position; }
	const Vector3& get_rotation() const { return _rotation; }
	void set_rotation(const Vector3& rotation) { _rotation = rotation; }
	const Vector3& get_scale() const { return _scale; }
	void set_scale(const Vector3& scale) { _scale = scale; }

private:
	unsigned int _vao{ 0 };
	unsigned int _vbo{ 0 };
	unsigned int _ebo{ 0 };
	size_t _vertex_count{ 0 };
	size_t _index_count{ 0 };

	ShaderProgram* _shader{ nullptr };
	Texture* _diffuse_texture{ nullptr };
	Texture* _specular_texture{ nullptr };
	Vector3 _position{ 0.0f, 0.0f, 0.0f };
	Vector3 _rotation{ 0.0f, 0.0f, 0.0f };
	Vector3 _scale{ 1.0f, 1.0f, 1.0f };
};
