#pragma once
#include <vector>

class Texture
{
public:
	Texture();
	virtual ~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture&&) = delete;

	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) = delete;

	bool load(const std::string& path, bool genMipmap = true);
	void unload();

	void active() const;

	unsigned int get_id() const { return _id; }
	size_t get_width() const { return _width; }
	size_t get_height() const { return _height; }

private:
	unsigned int _id;
	size_t _width;
	size_t _height;
};
