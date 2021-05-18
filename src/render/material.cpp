#include "material.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"
#include "glad/glad.h"
#include "graphic_api.h"

decltype(GL_ONE) convert_blend_factor(AlphaBlendFactor factor)
{
	switch (factor)
	{
	case AlphaBlendFactor::ZERO: return GL_ZERO;
	case AlphaBlendFactor::ONE: return GL_ONE;
	case AlphaBlendFactor::SRC_COLOR: return GL_SRC_COLOR;
	case AlphaBlendFactor::ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
	case AlphaBlendFactor::DST_COLOR: return GL_DST_COLOR;
	case AlphaBlendFactor::ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
	case AlphaBlendFactor::SRC_ALPHA: return GL_SRC_ALPHA;
	case AlphaBlendFactor::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
	case AlphaBlendFactor::DST_ALPHA: return GL_DST_ALPHA;
	case AlphaBlendFactor::ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
	case AlphaBlendFactor::CONSTANT_COLOR: return GL_CONSTANT_COLOR;
	case AlphaBlendFactor::ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR;
	case AlphaBlendFactor::CONSTANT_ALPHA: return GL_CONSTANT_ALPHA;
	case AlphaBlendFactor::ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA;
	default: assert(false); return GL_ZERO;
	}
}

void Material::active(const Matrix4& model) const
{
	if (_enable_depth_test)
	{
		CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
		CHECK_GL_ERROR(glDepthMask(_update_depth_value ? GL_TRUE : GL_FALSE));
		switch (_depth_test_func)
		{
		case DepthTestFunc::ALWAYS:   CHECK_GL_ERROR(glDepthFunc(GL_ALWAYS)); break;
		case DepthTestFunc::NEVER:    CHECK_GL_ERROR(glDepthFunc(GL_NEVER)); break;
		case DepthTestFunc::LESS:     CHECK_GL_ERROR(glDepthFunc(GL_LESS)); break;
		case DepthTestFunc::EQUAL:    CHECK_GL_ERROR(glDepthFunc(GL_EQUAL)); break;
		case DepthTestFunc::LEQUAL:   CHECK_GL_ERROR(glDepthFunc(GL_LEQUAL)); break;
		case DepthTestFunc::GREATER:  CHECK_GL_ERROR(glDepthFunc(GL_GREATER)); break;
		case DepthTestFunc::NOTEQUAL: CHECK_GL_ERROR(glDepthFunc(GL_NOTEQUAL)); break;
		case DepthTestFunc::GEQUAL:   CHECK_GL_ERROR(glDepthFunc(GL_GEQUAL)); break;
		default: break;
		}		
	}
	else
	{
		CHECK_GL_ERROR(glDisable(GL_DEPTH_TEST));
	}

	if (_cull_face_type != CullFaceType::NONE)
	{
		CHECK_GL_ERROR(glEnable(GL_CULL_FACE));
		switch (_cull_face_type)
		{
		default:
		case CullFaceType::BACK: CHECK_GL_ERROR(glCullFace(GL_BACK)); break;
		case CullFaceType::FRONT: CHECK_GL_ERROR(glCullFace(GL_FRONT)); break;
		case CullFaceType::ALL: CHECK_GL_ERROR(glCullFace(GL_FRONT_AND_BACK)); break;
		}
	}
	else
	{
		CHECK_GL_ERROR(glDisable(GL_CULL_FACE));
	}

	CHECK_GL_ERROR(glFrontFace(_clockwise_winding_order ? GL_CW : GL_CCW));

	if (_translucence && _enable_alpha_blend)
	{
		CHECK_GL_ERROR(glEnable(GL_BLEND));
		const auto src = convert_blend_factor(_blend_src_factor);
		const auto dst = convert_blend_factor(_blend_dst_factor);
		CHECK_GL_ERROR(glBlendFunc(src, dst));
	}
	else
	{
		CHECK_GL_ERROR(glDisable(GL_BLEND));
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
