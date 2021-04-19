#pragma once

#include "shader.h"
#include "texture.h"
#include "renderer.h"
#include "glad/glad.h"
#include "graphic_api.h"

class Material
{
public:
	Material(std::string name, ShaderProgram* shader,
		std::vector<Texture*> diffuse_textures = std::vector<Texture*>(),
		std::vector<Texture*> specular_textures = std::vector<Texture*>(),
		std::vector<Texture*> normal_textures = std::vector<Texture*>(),
		std::vector<Texture*> height_textures = std::vector<Texture*>())
		: _name(name)
		, _shader(shader)
		, _diffuse_textures(std::move(diffuse_textures))
		, _specular_textures(std::move(specular_textures))
		, _normal_textures(std::move(normal_textures))
		, _height_textures(std::move(height_textures))
	{
	}
	~Material() = default;

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

	void active(const Matrix4& model) const
	{
		_shader->bind();
		Renderer::get_singleton().bind_shader_data(*_shader);
		int n = 0;
		bind_textures(_diffuse_textures, "material.diffuse", n);
		bind_textures(_specular_textures, "material.specular", n);
		bind_textures(_normal_textures, "material.normal", n);
		bind_textures(_height_textures, "material.height", n);

		//model = glm::rotate(model, glm::radians(engine.get_time() * 30), glm::vec3(1.0f, 0.3f, 0.5f));
		_shader->set_matrix4("model", model);

	}

	void deactive() const
	{
		CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
		CHECK_GL_ERROR(glUseProgram(0));
	}

private:
	void bind_textures(const std::vector<Texture*>& textures, const std::string& prefix, int& n) const
	{
		for (size_t i = 0; i < textures.size(); ++i)
		{
			textures[i]->active(n);
			_shader->set_int(prefix + "_textures[" + std::to_string(i) + "]", n++);
		}
		_shader->set_int(prefix + "_count", textures.size());
	}

	std::string _name;
	ShaderProgram* _shader;
	std::vector<Texture*> _diffuse_textures{ };
	std::vector<Texture*> _specular_textures{ };
	std::vector<Texture*> _normal_textures{ };
	std::vector<Texture*> _height_textures{ };
	float _specular_shininess{ 64.0f };
};
