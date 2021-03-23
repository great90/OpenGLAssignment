#pragma once
#include <vector>

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
	
private:
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;
	size_t _vertex_count;
	size_t _index_count;
};
