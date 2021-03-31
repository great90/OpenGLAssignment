#include "shader.h"
#include "glad/glad.h"
#include <cassert>

ShaderObject::ShaderObject(Type type, std::string source)
{
	_type = type;
	switch (type)
	{
	case Type::Vertex:
		_id = glCreateShader(GL_VERTEX_SHADER);
		break;
	case Type::Fragment:
		_id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		assert(false);
		return;
	}
	_source = source;

	const char* src = _source.c_str();
	glShaderSource(_id, 1, &src, NULL);
	glCompileShader(_id);

	int success = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		_compile_log.resize(512);
		glGetShaderInfoLog(_id, _compile_log.capacity(), NULL, &_compile_log[0]);
		_valid = false;
	}
	else
	{
		_compile_log = "";
		_valid = true;
	}
}

ShaderObject::~ShaderObject()
{
	glDeleteShader(_id);
}


ShaderProgram::ShaderProgram(const std::vector<const ShaderObject*>& shaders)
{
	assert(shaders.size() >= 2);

	_id = glCreateProgram();
	for (auto shader : shaders)
	{
		assert(shader->valid());
		glAttachShader(_id, shader->_id);
	}
	glLinkProgram(_id);

	int success = 0;
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		_link_log.resize(512);
		glGetProgramInfoLog(_id, _link_log.capacity(), NULL, &_link_log[0]);
		_valid = false;
	}
	else
	{
		_link_log = "";
		_valid = true;
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_id);
}

void ShaderProgram::set_bool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}

void ShaderProgram::set_int(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void ShaderProgram::set_float(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void ShaderProgram::set_vector2(const std::string& name, const Vector2& value) const
{
	glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::set_vector3(const std::string& name, const Vector3& value) const
{
	glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::set_vector4(const std::string& name, const Vector4& value) const
{
	glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::set_vector3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
}

void ShaderProgram::set_vector4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w);
}

void ShaderProgram::set_matrix3(const std::string& name, const Matrix3& value) const
{
	glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::set_matrix4(const std::string& name, const Matrix4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::bind() const
{
	glUseProgram(_id);
}

void ShaderProgram::unbind() const
{
	glUseProgram(0);
}
