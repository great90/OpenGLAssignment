#include "material.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"
#include "glad/glad.h"
#include "graphic_api.h"

void Material::active(const Matrix4& model) const
{
	if (_enable_depth_test)
	{
		CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
		CHECK_GL_ERROR(glDepthMask(_update_depth_value ? GL_TRUE : GL_FALSE));
		switch (_depth_test_func)
		{
		case DepthTestFunc::ALWAYS:   glDepthFunc(GL_ALWAYS); break;
		case DepthTestFunc::NEVER:    glDepthFunc(GL_NEVER); break;
		case DepthTestFunc::LESS:     glDepthFunc(GL_LESS); break;
		case DepthTestFunc::EQUAL:    glDepthFunc(GL_EQUAL); break;
		case DepthTestFunc::LEQUAL:   glDepthFunc(GL_LEQUAL); break;
		case DepthTestFunc::GREATER:  glDepthFunc(GL_GREATER); break;
		case DepthTestFunc::NOTEQUAL: glDepthFunc(GL_NOTEQUAL); break;
		case DepthTestFunc::GEQUAL:   glDepthFunc(GL_GEQUAL); break;
		default: break;
		}		
	}
	else
	{
		CHECK_GL_ERROR(glDisable(GL_DEPTH_TEST));
	}
	if (_cull_back_faces)
	{
		CHECK_GL_ERROR(glEnable(GL_CULL_FACE));
	}
	else
	{
		CHECK_GL_ERROR(glDisable(GL_CULL_FACE));
	}

	_shader->bind();
	Renderer::get_singleton().bind_shader_data(*_shader);
	int n = 0;
	bind_textures(_diffuse_textures, "material.diffuse", n);
	bind_textures(_specular_textures, "material.specular", n);
	bind_textures(_normal_textures, "material.normal", n);
	bind_textures(_height_textures, "material.height", n);

	_shader->set_matrix4("model", model);

}

void Material::deactive() const
{
	//CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
	CHECK_GL_ERROR(glUseProgram(0));
}

void Material::bind_textures(const std::vector<Texture*>& textures, const std::string& prefix, int& n) const
{
	for (size_t i = 0; i < textures.size(); ++i)
	{
		textures[i]->active(n);
		_shader->set_int(prefix + "_textures[" + std::to_string(i) + "]", n++);
	}
	_shader->set_int(prefix + "_count", textures.size());
}
