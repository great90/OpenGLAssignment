#pragma once
#include "mesh.h"

#include "assimp/scene.h"

struct aiMaterial;
struct aiNode;

class Model
{
public:
	Model(const std::string& path, bool gamma = false)
		: _gamma_correction(gamma)
	{
		load_model(path);
	}
	~Model() = default;

	void draw()
	{
		const auto model = get_model_matrix();
		for (auto& mesh : _meshes)
			mesh->draw(model);
	}

	const Vector3& get_position() const { return _position; }
	void set_position(const Vector3& position) { _position = position; }
	const Vector3& get_rotation() const { return _rotation; }
	void set_rotation(const Vector3& rotation) { _rotation = rotation; }
	const Vector3& get_scale() const { return _scale; }
	void set_scale(const Vector3& scale) { _scale = scale; }

private:
	void load_model(const std::string& path);
	void process_node(aiNode* node, const aiScene* scene);
	Mesh* process_mesh(aiMesh* mesh, const aiScene* scene) const;
	std::vector<Texture*> load_material_textures(aiMaterial* material, aiTextureType type) const;
	Matrix4 get_model_matrix() const;
	
	std::vector<Mesh*> _meshes;
	std::string _path;
	bool _gamma_correction;
	
	Vector3 _position{ 0.0f, 0.0f, 0.0f };
	Vector3 _rotation{ 0.0f, 0.0f, 0.0f };
	Vector3 _scale{ 1.0f, 1.0f, 1.0f };
};
