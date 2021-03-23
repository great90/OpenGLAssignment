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

void ShaderProgram::bind() const
{
	glUseProgram(_id);
}

void ShaderProgram::unbind() const
{
	glUseProgram(0);
}
