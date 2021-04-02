#pragma once
#include <vector>
#include "math/math.h"
#include <glm/ext/matrix_transform.inl>

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

	Matrix4 get_model_matrix() const { return glm::translate(glm::mat4(1.0f), _position); }
	
	ShaderProgram* get_shader() const { return _shader; }
	void set_shader(ShaderProgram* shader) { _shader = shader; }
	Texture* get_texture() const { return _texture; }
	void set_texture(Texture* texture) { _texture = texture; }
	const Vector3& get_position() const { return _position; }
	void set_position(const Vector3& position) { _position = position; }
	
private:
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;
	size_t _vertex_count;
	size_t _index_count;

	ShaderProgram* _shader;
	Texture* _texture;
	Vector3 _position;
};
