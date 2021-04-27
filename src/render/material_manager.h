#pragma once

#include <string>
#include <vector>
#include "math/math.h"
#include "common/singleton.h"
#include "material.h"
#include <map>
#include <iostream>

class MaterialManager : public Singleton<MaterialManager>
{
public:
	MaterialManager() = default;
	~MaterialManager() { cleanup(); }

	MaterialManager(const MaterialManager&) = delete;
	MaterialManager(MaterialManager&&) = delete;

	MaterialManager& operator=(const MaterialManager&) = delete;
	MaterialManager& operator=(MaterialManager&&) = delete;

	Material* create_material(const std::string& name, ShaderProgram* shader,
		std::vector<Texture*> diffuse_textures = std::vector<Texture*>(),
		std::vector<Texture*> specular_textures = std::vector<Texture*>(),
		std::vector<Texture*> normal_textures = std::vector<Texture*>(),
		std::vector<Texture*> height_textures = std::vector<Texture*>())
	{
		assert(shader != nullptr);
		assert(!get_material(name));

		Material* material = new Material(name, shader, diffuse_textures, specular_textures, normal_textures, height_textures);
		_materials[name] = material;
		return material;
	}

	Material* get_material(const std::string& name) const
	{
		const auto iter = _materials.find(name);
		return iter != _materials.end() ? iter->second : nullptr;
	}
	
	void cleanup()
	{
		for (auto& pair : _materials)
		{
			delete pair.second;
		}
		_materials.clear();
	}

private:
	std::map<std::string, Material*> _materials{ };
};
