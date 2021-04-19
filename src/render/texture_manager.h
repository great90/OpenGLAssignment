#pragma once

#include <string>
#include <vector>
#include "math/math.h"
#include "common/singleton.h"
#include "shader.h"
#include <map>
#include "texture.h"

class TextureManager : public Singleton<TextureManager>
{
public:
	TextureManager() = default;
	~TextureManager() { cleanup(); }

	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;

	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;

	bool has_texture(const std::string& path) const { return _textures.find(path) != _textures.end(); }

	Texture* get_texture(const std::string& path) const
	{
		const auto iter = _textures.find(path);
		return iter != _textures.end() ? iter->second : nullptr;
	}

	Texture* load_texture(const std::string& path)
	{
		assert(!has_texture(path));
		auto texture = new Texture();
		if (!texture->load(path))
		{
			delete texture;
			return nullptr;
		}
		_textures[path] = texture;
		return texture;
	}

	void cleanup()
	{
		for (auto& pair : _textures)
		{
			delete pair.second;
		}
		_textures.clear();
	}

private:
	std::map<std::string, Texture*> _textures{ };
};
