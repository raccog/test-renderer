#include "shaders.h"

#include <iostream>

#include "glad/glad.h"

Shader Shader::createProgram(const std::string &vertexSource, const std::string &fragmentSource) {
	// compile vertex shader source code
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
		// TODO: log here
		std::cout << "Vertex shader failed to compile\n";
		throw ShaderCompileError(ShaderType::Vertex);
	}

	// compile vertex shader source code
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fragmentSource_c_str = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSource_c_str, nullptr);
	glCompileShader(fragmentShader);

	// check for compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		// TODO: log here
		std::cout << "Fragment shader failed to compile\n";
		throw ShaderCompileError(ShaderType::Fragment);
	}

	// link final shader program
	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		// TODO: log here
		std::cout << "Shader program failed to link\n";
		throw ShaderLinkError();
	}

	// individual shaders are not needed after the final
	// program has successfully been linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return Shader(shaderProgram);
}

void Shader::bind() const {
	glUseProgram(m_id);
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
