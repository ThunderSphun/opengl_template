#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "shader.hpp"

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	ShaderProgram* attachShader(const Shader& shader);
	ShaderProgram* bindAttribute(int index, const char* name);
	ShaderProgram* linkProgram();
	ShaderProgram* useProgram();

	void update();

	[[nodiscard]] GLint getUniform(const std::string& uniform) const;

	void setUniform(const std::string& uniform, bool value) const;
	void setUniform(const std::string& uniform, int value) const;
	void setUniform(const std::string& uniform, float value) const;
	void setUniform(const std::string& uniform, const glm::mat4& value) const;
	void setUniform(const std::string& uniform, const glm::mat3& value) const;
	void setUniform(const std::string& uniform, const glm::vec4& value) const;
	void setUniform(const std::string& uniform, const glm::vec3& value) const;
	void setUniform(const std::string& uniform, const glm::vec2& value) const;

	void setUniform(GLint uniform, bool value) const;
	void setUniform(GLint uniform, int value) const;
	void setUniform(GLint uniform, float value) const;
	void setUniform(GLint uniform, const glm::mat4& value) const;
	void setUniform(GLint uniform, const glm::mat3& value) const;
	void setUniform(GLint uniform, const glm::vec4& value) const;
	void setUniform(GLint uniform, const glm::vec3& value) const;
	void setUniform(GLint uniform, const glm::vec2& value) const;

private:
	std::vector<Shader> shaders;
	GLuint id = -1;
};