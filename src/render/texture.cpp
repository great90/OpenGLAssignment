#include "texture.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <ostream>
#include <iostream>

Texture::Texture()
	: _id(0)
	, _width(0)
	, _height(0)
{	
}

Texture::~Texture()
{
	unload();
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
	assert(channels == 3 || channels == 4);

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	const unsigned int format = channels == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	if (genMipmap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	return true;
}

void Texture::unload()
{
	if (_id)
	{
		glDeleteTextures(1, &_id);
		_id = 0;
	}
}

void Texture::active() const
{
	glBindTexture(GL_TEXTURE_2D, _id);
}
