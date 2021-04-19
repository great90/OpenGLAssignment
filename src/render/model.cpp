#include "model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include <iostream>
#include "texture.h"
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/quaternion.hpp>
#include "texture_manager.h"
#include "shader_manager.h"

void Model::load_model(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Assimp load model error: " << importer.GetErrorString() << std::endl;
		return;
	}
	_path = path;
	process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(process_mesh(mesh, scene));
	}
	for (size_t i = 0; i < node->mNumChildren; ++i)
	{
		process_node(node->mChildren[i], scene);
	}
}

Mesh* Model::process_mesh(aiMesh* mesh, const aiScene* scene) const
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	for (size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		Mesh::Vertex vertex;
		vertex.position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->HasNormals())
		{
			vertex.normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		else
		{
			vertex.normal = Vector3(0.0f, 0.0f, 0.0f);
		}
		if (mesh->mTextureCoords[0])
		{
			vertex.uv = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			vertex.tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.bitangent = Vector3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		else
		{
			vertex.uv = Vector2(0.0f, 0.0f);
			vertex.tangent = Vector3(0.0f, 0.0f, 0.0f);
			vertex.bitangent = Vector3(0.0f, 0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	const std::vector<Texture*> diffuse_textures = load_material_textures(material, aiTextureType_DIFFUSE);
	const std::vector<Texture*> specular_textures = load_material_textures(material, aiTextureType_SPECULAR);
	const std::vector<Texture*> normal_textures = load_material_textures(material, aiTextureType_HEIGHT);
	const std::vector<Texture*> height_textures = load_material_textures(material, aiTextureType_AMBIENT);

	ShaderProgram* shader = ShaderManager::get_singleton().get_program("mesh");
	assert(shader && shader->valid());

	Material* mat = new Material(material->GetName().C_Str(), shader, diffuse_textures, specular_textures, normal_textures, height_textures);
	return new Mesh(vertices, indices, mat);
}

std::vector<Texture*> Model::load_material_textures(aiMaterial* material, aiTextureType type) const
{
	std::vector<Texture*> textures;
	for (size_t i = 0; i < material->GetTextureCount(type); ++i)
	{
		aiString str;
		if (material->GetTexture(type, i, &str) != aiReturn_SUCCESS)
		{
			std::cout << "Assimp load material textures type [" << type << "] error: " << material->GetName().C_Str() << std::endl;
			continue;
		}
		const std::string path = _path.substr(0, _path.find_last_of('/')) + "/" + str.C_Str();
		if (!TextureManager::get_singleton().has_texture(path))
		{
			if (auto tex = TextureManager::get_singleton().load_texture(path))
				textures.push_back(tex);
		}
	}
	return textures;
}

Matrix4 Model::get_model_matrix() const
{
	const Quaternion q = Quaternion(glm::radians(glm::vec3(_rotation.x, _rotation.y, _rotation.z)));
	Matrix4 model = glm::mat4(1.0f);
	model = glm::scale(model, _scale);
	model = glm::mat4_cast(q) * model;
	return glm::translate(model, _position);
}