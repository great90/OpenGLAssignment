#pragma once
#include <vector>
#include "math/math.h"
#include "metrial.h"

class ShaderProgram;
class Texture;

class Mesh
{
public:
	struct Vertex
	{
		Vector3 position{};
		Vector3 normal{};
		Vector2 uv{};
		Vector3 tangent{};
		Vector3 bitangent{};
	};
	
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material);

	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;

	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&&) = delete;
	
	~Mesh();

	void draw(const Matrix4& model) const;

private:
	void setup();

	unsigned int _vao{ 0 };
	unsigned int _vbo{ 0 };
	unsigned int _ebo{ 0 };
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	Material* _material;
};
