#include "shaders.h"

#include <iostream>

#include "glad/glad.h"
#include "spdlog/spdlog.h"

Shader Shader::createProgram(const std::string &vertexSource, const std::string &fragmentSource) {
	// compile vertex shader source code
    spdlog::debug("Compiling vertex shader");
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char *vertexSource_c_str = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vertexSource_c_str, nullptr);
	glCompileShader(vertexShader);

	// check for compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        spdlog::critical("Vertex shader failed to compile: {}", infoLog);
		throw ShaderCompileError(ShaderType::Vertex);
	}

	// compile fragment shader source code
    spdlog::debug("Compiling fragment shader");
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fragmentSource_c_str = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSource_c_str, nullptr);
	glCompileShader(fragmentShader);

	// check for compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        spdlog::critical("Fragment shader failed to compile: {}", infoLog);
		throw ShaderCompileError(ShaderType::Fragment);
	}

	// link final shader program
    spdlog::debug("Linking final shader program");
	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        spdlog::critical("Shader program linking failed: {}", infoLog);
		throw ShaderLinkError();
	}

	// individual shaders are not needed after the final
	// program has successfully been linked
    spdlog::debug("Shader program successfully compiled");
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return Shader(shaderProgram);
}

void Shader::bind() const {
	glUseProgram(m_id);
}

void Shader::deleteShader() {
    glDeleteProgram(m_id);
    m_id = 0;
}

void Shader::unbind() const {
	glUseProgram(0);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setFloat3(const std::string &name, const glm::vec3 &vec) const {
	glUniform3f(glGetUniformLocation(m_id, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
