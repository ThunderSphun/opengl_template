#include "shaderProgram.hpp"
#include "glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram() {
	id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
	if (id != -1) {
		glDeleteProgram(id);
	}
}

ShaderProgram* ShaderProgram::attachShader(const Shader& shader) {
	shaders.push_back(shader);
	glAttachShader(id, shader.id);

	return this;
}

ShaderProgram* ShaderProgram::bindAttribute(int index, const char* name) {
	glBindAttribLocation(id, index, name);

	return this;
}

ShaderProgram* ShaderProgram::linkProgram() {
	glLinkProgram(id);

	// TODO: check for linking errors

	return this;
}

ShaderProgram* ShaderProgram::useProgram() {
	glUseProgram(id);

	return this;
}

void ShaderProgram::update() {
	for (const auto& shader : shaders) {
		if (shader.shouldUpdate()) {

		}
	}
}

GLint ShaderProgram::getUniform(const std::string& uniform) const {
	return glGetUniformLocation(id, uniform.c_str());
}

void ShaderProgram::setUniform(const std::string& uniform, const glm::mat4& value) const {
	setUniform(getUniform(uniform), value);
}

void ShaderProgram::setUniform(const std::string& uniform, const glm::mat3& value) const {
	setUniform(getUniform(uniform), value);
}

void ShaderProgram::setUniform(const std::string& uniform, const glm::vec4& value) const {
	setUniform(getUniform(uniform), value);
}

void ShaderProgram::setUniform(const std::string& uniform, const glm::vec3& value) const {
	setUniform(getUniform(uniform), value);
}

void ShaderProgram::setUniform(const std::string& uniform, const glm::vec2& value) const {
	setUniform(getUniform(uniform), value);
}

void ShaderProgram::setUniform(const std::string& uniform, bool value) const {
	setUniform(getUniform(uniform), value);
}

void ShaderProgram::setUniform(const std::string& uniform, int value) const {
	setUniform(getUniform(uniform), value);
}

void ShaderProgram::setUniform(const std::string& uniform, float value) const {
	setUniform(getUniform(uniform), value);
}

void ShaderProgram::setUniform(GLint uniform, bool value) const {
	glUniform1i(uniform, value ? GL_TRUE : GL_FALSE);
}

void ShaderProgram::setUniform(GLint uniform, int value) const {
	glUniform1i(uniform, value);
}

void ShaderProgram::setUniform(GLint uniform, float value) const {
	glUniform1f(uniform, value);
}

void ShaderProgram::setUniform(GLint uniform, const glm::mat4& value) const {
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniform(GLint uniform, const glm::mat3& value) const {
	glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniform(GLint uniform, const glm::vec4& value) const {
	glUniform4fv(uniform, 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(GLint uniform, const glm::vec3& value) const {
	glUniform3fv(uniform, 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(GLint uniform, const glm::vec2& value) const {
	glUniform2fv(uniform, 1, glm::value_ptr(value));
}

