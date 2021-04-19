#include "shader.h"
#include "glad/glad.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include "graphic_api.h"

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
	CHECK_GL_ERROR(glShaderSource(_id, 1, &src, NULL));
	CHECK_GL_ERROR(glCompileShader(_id));

	int success = 0;
	CHECK_GL_ERROR(glGetShaderiv(_id, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		_compile_log.resize(512);
		CHECK_GL_ERROR(glGetShaderInfoLog(_id, _compile_log.capacity(), NULL, &_compile_log[0]));
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
	CHECK_GL_ERROR(glDeleteShader(_id));
}


ShaderProgram::ShaderProgram(const std::vector<const ShaderObject*>& shaders)
{
	assert(shaders.size() >= 2);

	_id = glCreateProgram();
	for (auto shader : shaders)
	{
		assert(shader->valid());
		CHECK_GL_ERROR(glAttachShader(_id, shader->_id));
	}
	CHECK_GL_ERROR(glLinkProgram(_id));

	int success = 0;
	CHECK_GL_ERROR(glGetProgramiv(_id, GL_LINK_STATUS, &success));
	if (!success)
	{
		_error_log.resize(512);
		CHECK_GL_ERROR(glGetProgramInfoLog(_id, _error_log.capacity(), NULL, &_error_log[0]));
		_valid = false;
	}
	else
	{
		_error_log = "";
		_valid = true;
	}
}

ShaderProgram::ShaderProgram(const std::string& vertex_path, const std::string& fragment_path)
{
	_id = glCreateProgram();

	const unsigned int vs = load_shader_file(ShaderObject::Type::Vertex, vertex_path, _error_log);
	if (vs == 0)
	{
		_valid = false;
		return;
	}
	const unsigned int fs = load_shader_file(ShaderObject::Type::Fragment, fragment_path, _error_log);
	if (fs == 0)
	{
		_valid = false;
		return;
	}
	CHECK_GL_ERROR(glAttachShader(_id, vs));
	CHECK_GL_ERROR(glAttachShader(_id, fs));
	CHECK_GL_ERROR(glLinkProgram(_id));

	int success = 0;
	CHECK_GL_ERROR(glGetProgramiv(_id, GL_LINK_STATUS, &success));
	if (!success)
	{
		_error_log.resize(512);
		CHECK_GL_ERROR(glGetProgramInfoLog(_id, _error_log.capacity(), NULL, &_error_log[0]));
		_valid = false;
	}
	else
	{
		_error_log = "";
		_valid = true;
	}
}

ShaderProgram::~ShaderProgram()
{
	CHECK_GL_ERROR(glDeleteProgram(_id));
}

void ShaderProgram::set_bool(const std::string& name, bool value) const
{
	CHECK_GL_ERROR(glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value));
}

void ShaderProgram::set_int(const std::string& name, int value) const
{
	CHECK_GL_ERROR(glUniform1i(glGetUniformLocation(_id, name.c_str()), value));
}

void ShaderProgram::set_float(const std::string& name, float value) const
{
	CHECK_GL_ERROR(glUniform1f(glGetUniformLocation(_id, name.c_str()), value));
}

void ShaderProgram::set_vector2(const std::string& name, const Vector2& value) const
{
	CHECK_GL_ERROR(glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]));
}

void ShaderProgram::set_vector3(const std::string& name, const Vector3& value) const
{
	CHECK_GL_ERROR(glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]));
}

void ShaderProgram::set_vector4(const std::string& name, const Vector4& value) const
{
	CHECK_GL_ERROR(glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]));
}

void ShaderProgram::set_vector3(const std::string& name, float x, float y, float z) const
{
	CHECK_GL_ERROR(glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z));
}

void ShaderProgram::set_vector4(const std::string& name, float x, float y, float z, float w) const
{
	CHECK_GL_ERROR(glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w));
}

void ShaderProgram::set_matrix3(const std::string& name, const Matrix3& value) const
{
	CHECK_GL_ERROR(glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &value[0][0]));
}

void ShaderProgram::set_matrix4(const std::string& name, const Matrix4& value) const
{
	CHECK_GL_ERROR(glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &value[0][0]));
}

void ShaderProgram::bind() const
{
	CHECK_GL_ERROR(glUseProgram(_id));
}

void ShaderProgram::unbind() const
{
	CHECK_GL_ERROR(glUseProgram(0));
}

unsigned int ShaderProgram::load_shader_file(ShaderObject::Type type, const std::string& path, std::string& error_log) const
{
	unsigned int id = 0;
	switch (type)
	{
	case ShaderObject::Type::Vertex:
		id = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ShaderObject::Type::Fragment:
		id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		assert(false);
		return 0;
	}

	std::string source;
	std::ifstream stream;
	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		stream.open(path);
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();
		source = ss.str();
	}
	catch (std::ifstream::failure& e)
	{
		error_log = "read file '" + path + "' failed: " + e.what();
		return 0;
	}

	const char* src = source.c_str();
	CHECK_GL_ERROR(glShaderSource(id, 1, &src, NULL));
	CHECK_GL_ERROR(glCompileShader(id));

	int success = 0;
	CHECK_GL_ERROR(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		error_log.resize(512);
		CHECK_GL_ERROR(glGetShaderInfoLog(id, error_log.capacity(), NULL, &error_log[0]));
		return 0;
	}
	return id;
}
