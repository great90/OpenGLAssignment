#pragma once
#include <vector>
#include <map>

class Texture
{
	friend class TextureManager;
public:
	virtual ~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture&&) = delete;

	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) = delete;

	void active(unsigned char index = 0) const;

	size_t get_width() const { return _width; }
	size_t get_height() const { return _height; }
	const std::string& get_path() const { return _path; }

protected:
	Texture();
	bool load(const std::string& path, bool genMipmap = true);

private:
	unsigned int _id;
	size_t _width;
	size_t _height;
	std::string _path;
};
