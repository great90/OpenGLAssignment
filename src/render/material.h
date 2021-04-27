#pragma once

#include <string>
#include <utility>
#include <vector>
#include "math/math.h"

class Texture;
class ShaderProgram;

class Material
{
	friend class MaterialManager;
public:
	~Material() = default;

	Material(const Material&) = delete;
	Material(Material&&) = delete;

	Material& operator=(const Material&) = delete;
	Material& operator=(Material&&) = delete;

	bool operator==(const Material& rhs) const
	{
		return _shader == rhs._shader &&
			_diffuse_textures == rhs._diffuse_textures &&
			_specular_textures == rhs._specular_textures &&
			_normal_textures == rhs._normal_textures &&
			_height_textures == rhs._height_textures &&
			_specular_shininess == rhs._specular_shininess;
	}
	bool operator!=(const Material& rhs) const { return !operator==(rhs); }

	bool operator<(const Material& rhs) const
	{
		return _shader < rhs._shader &&
			_diffuse_textures < rhs._diffuse_textures &&
			_specular_textures < rhs._specular_textures &&
			_normal_textures < rhs._normal_textures &&
			_height_textures < rhs._height_textures &&
			_specular_shininess < rhs._specular_shininess;
	}
	bool operator<=(const Material& rhs) const { return operator<(rhs) || operator==(rhs); }
	bool operator>(const Material& rhs) const { return !operator<=(rhs); }
	bool operator>=(const Material& rhs) const { return !operator<(rhs); }

	const std::string& get_name() const { return _name; }
	ShaderProgram* get_shader() const { return _shader; }
	void set_shader(ShaderProgram* shader) { _shader = shader; }
	const std::vector<Texture*>& get_diffuse_textures() const { return _diffuse_textures; }
	const std::vector<Texture*>& get_specular_textures() const { return _specular_textures; }
	const std::vector<Texture*>& get_normal_textures() const { return _normal_textures; }
	const std::vector<Texture*>& get_height_textures() const { return _height_textures; }
	void set_enable_depth_test(bool enable) { _enable_depth_test = enable; }
	bool is_enable_depth_test() const { return _enable_depth_test; }
	void set_cull_back_faces(bool cull) { _cull_back_faces = cull; }
	bool is_cull_back_faces() const { return _cull_back_faces; }

	void active(const Matrix4& model) const;
	void deactive() const;

private:
	Material(std::string name, ShaderProgram* shader,
		std::vector<Texture*> diffuse_textures = std::vector<Texture*>(),
		std::vector<Texture*> specular_textures = std::vector<Texture*>(),
		std::vector<Texture*> normal_textures = std::vector<Texture*>(),
		std::vector<Texture*> height_textures = std::vector<Texture*>())
		: _name(std::move(name))
		, _shader(shader)
		, _diffuse_textures(std::move(diffuse_textures))
		, _specular_textures(std::move(specular_textures))
		, _normal_textures(std::move(normal_textures))
		, _height_textures(std::move(height_textures))
	{
	}

	void bind_textures(const std::vector<Texture*>& textures, const std::string& prefix, int& n) const;

	std::string _name;
	ShaderProgram* _shader;
	std::vector<Texture*> _diffuse_textures{ };
	std::vector<Texture*> _specular_textures{ };
	std::vector<Texture*> _normal_textures{ };
	std::vector<Texture*> _height_textures{ };
	float _specular_shininess{ 64.0f };

	bool _enable_depth_test{ true };
	bool _cull_back_faces{ true };
};
