#pragma once

#include <cstdint>
#include <exception>
#include <sstream>
#include <string>

class Shader {
	// opengl shader program handle
	uint32_t m_id;

public:
	Shader(uint32_t id) : m_id{id} {}

	static Shader createProgram(const std::string &vertexSource, const std::string &fragmentSource);

	void bind() const;

	inline uint32_t id() const {
		return m_id;
	}

	void setBool(const std::string &name, bool value) const;

	void setInt(const std::string &name, int value) const;

	void setFloat(const std::string &name, float value) const;

	void unbind() const;
};

enum class ShaderType {
	Vertex,
	Fragment,
};

inline std::string shaderTypeToString(ShaderType shaderType) {
	switch (shaderType) {
		case ShaderType::Vertex:
			return "Vertex";
		case ShaderType::Fragment:
			return "Fragment";
	}
	return "Unknown";
}

class ShaderCompileError: public std::runtime_error {
public:
	ShaderCompileError(ShaderType shaderType)
		: std::runtime_error{shaderTypeToString(shaderType)} {}
};

class ShaderLinkError: public std::runtime_error {
public:
	ShaderLinkError()
		: std::runtime_error{"Shader Program"} {}
};
