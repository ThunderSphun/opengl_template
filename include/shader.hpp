#pragma once

#include <string>
#include <fstream>
#include <optional>
#include <glad/gl.h>

enum class ShaderType;

class Shader {
public:
	Shader(const std::string& path, ShaderType shaderType);
	~Shader();

	std::string lastError;

private:
	friend class ShaderProgram;

	bool compile(const char* source, ShaderType shaderType);

	[[nodiscard]] bool shouldUpdate() const;

	std::optional<std::string> path;
	GLuint id = -1;
};

enum class ShaderType {
	fragment,
	vertex
};