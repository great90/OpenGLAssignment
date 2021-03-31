#pragma once

#include <string>
#include <vector>
#include "math/math.h"

class ShaderObject final
{
	friend class ShaderProgram;
public:
	enum class Type : unsigned int
	{
		Vertex = 0,
		Fragment
	};

	ShaderObject(Type type, std::string source);
	~ShaderObject();

	ShaderObject(const ShaderObject&) = delete;
	ShaderObject(ShaderObject&&) = delete;

	ShaderObject& operator=(const ShaderObject&) = delete;
	ShaderObject& operator=(ShaderObject&&) = delete;

	const std::string& get_source() const { return _source; }
	const std::string& get_compile_log() const { return _compile_log; }
	bool valid() const { return _valid; }

private:
	Type _type;
	unsigned int _id;
	std::string _source;
	std::string _compile_log;
	bool _valid;
};

class ShaderProgram final
{
public:
	ShaderProgram(const std::vector<const ShaderObject*>& shaders);
	~ShaderProgram();

	const std::string& get_link_log() const { return _link_log; }
	bool valid() const { return _valid; }

	void set_bool(const std::string& name, bool value) const;
	void set_int(const std::string& name, int value) const;
	void set_float(const std::string& name, float value) const;

	void set_vector2(const std::string& name, const Vector2& value) const;
	void set_vector3(const std::string& name, const Vector3& value) const;
	void set_vector4(const std::string& name, const Vector4& value) const;
	void set_vector3(const std::string& name, float x, float y, float z) const;
	void set_vector4(const std::string& name, float x, float y, float z, float w) const;

	void set_matrix3(const std::string& name, const Matrix3& value) const;
	void set_matrix4(const std::string& name, const Matrix4& value) const;

	void bind() const;
	void unbind() const;

private:
	unsigned int _id;
	std::string _link_log;
	bool _valid;
};