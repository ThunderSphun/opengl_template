#include <iostream>
#include <cstring>
#include "shader.hpp"

Shader::Shader(const std::string& path, ShaderType shaderType) {
	this->path = std::string(path);

	std::ifstream file;
	if (path.find('\\') != std::string::npos || path.find('/') != std::string::npos)
		file.open(path);
	else
		file.open("resources/shaders/" + path);

	if (!file)
		throw "file path invalid";

	std::string shaderString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	if (shaderString.empty())
		throw "file wasn't loaded";

	if (!compile(shaderString.c_str(), shaderType))
		throw lastError;
}

Shader::~Shader() {
	if (id != -1)
		glDeleteShader(id);
}

bool Shader::shouldUpdate() const {
	if (!path.has_value()) {
		return false;
	}

	return false;
}

bool Shader::compile(const char* source, ShaderType shaderType) {
	switch (shaderType) {
		case ShaderType::fragment:
			id = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderType::vertex:
			id = glCreateShader(GL_VERTEX_SHADER);
			break;
		default:
			std::cout << "shader type not supported" << std::endl;
			return false;
	}
	if (glGetError() == GL_INVALID_ENUM) {
		std::cerr << "passed in wrong shader type" << std::endl;
		id = -1;
		return false;
	}
	if (id == 0) {
		id = -1;
		return false;
	}

	glShaderSource(id, 1, &source, nullptr);
	{
		GLint error;
		glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &error);
		if (error == 0) {
			glDeleteShader(id);
			id = -1;
			return false;
		}
	}

	glCompileShader(id);
	{
		GLint error;
		glGetShaderiv(id, GL_COMPILE_STATUS, &error);
		if (error == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* errorMessage = new char[length + 1];
			memset(errorMessage, 0, length + 1);
			int writtenCount;
			glGetShaderInfoLog(id, length, &writtenCount, errorMessage);
			lastError = std::string(errorMessage);
			delete[] errorMessage;

			glDeleteShader(id);
			return false;
		}
	}

	return true;
}
