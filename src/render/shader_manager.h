#pragma once

#include <string>
#include <vector>
#include "math/math.h"
#include "common/singleton.h"
#include "shader.h"
#include <map>
#include <iostream>

class ShaderManager : public Singleton<ShaderManager>
{
public:
	ShaderManager() = default;
	~ShaderManager() { cleanup(); }

	ShaderManager(const ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) = delete;

	ShaderManager& operator=(const ShaderManager&) = delete;
	ShaderManager& operator=(ShaderManager&&) = delete;

	ShaderProgram* get_program(const std::string& name) const
	{
		const auto iter = _programs.find(name);
		return iter != _programs.end() ? iter->second : nullptr;
	}
	
	ShaderProgram* load(const std::string& name, const std::string& vertex_path, const std::string& fragment_path)
	{
		assert(_programs.find(name) == _programs.end());

		const auto program = new ShaderProgram(vertex_path, fragment_path);
		if (!program->valid())
		{
			std::cout << "ShaderManager load failed, " << vertex_path << ", " << fragment_path << " : " << program->get_error_log() << std::endl;
			delete program;
			return nullptr;
		}
		_programs[name] = program;
		return program;
	}
	//bool create_from_source(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);

	void cleanup()
	{
		for (auto& pair : _programs)
		{
			delete pair.second;
		}
		_programs.clear();
	}

private:
	std::map<std::string, ShaderProgram*> _programs{ };
};
