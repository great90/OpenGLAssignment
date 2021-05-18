#include "texture.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <ostream>
#include <iostream>
#include "graphic_api.h"

Texture::Texture()
	: _id(0)
	, _width(0)
	, _height(0)
	, _path("")
{	
}

Texture::~Texture()
{
	if (_id)
	{
		CHECK_GL_ERROR(glDeleteTextures(1, &_id));
		_id = 0;
		_path = "";
	}
}

bool Texture::load(const std::string& path, bool genMipmap/*=true*/)
{
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		std::cout << "Failed to load texture: " << path.c_str() << std::endl;
		return false;
	}

	GLenum format;
	switch (channels)
	{
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default: assert(false); break;
	}

	_path = path;
	CHECK_GL_ERROR(glGenTextures(1, &_id));
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, _id));

	const auto wrap = format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT;
	CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap));
	CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap));
	CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	stbi_image_free(data);

	if (genMipmap)
	{
		CHECK_GL_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
	}
	return true;
}

void Texture::active(unsigned char index/*=0*/) const
{
	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0 + index));
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, _id));
}
