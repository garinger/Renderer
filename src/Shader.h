#pragma once

#include "glad/glad.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <vector>

class Shader
{
public:
	Shader(std::string vertexPath, std::string fragmentPath);
	~Shader();

	// Bind this shader to be used
	void Use() const { glUseProgram(m_ShaderProgram); };

	unsigned int GetShaderProgram() { return m_ShaderProgram; };

	// Used for assigning our view and projection matrix uniforms
	void SetUniform(std::string uniformName, glm::mat4 m) const;
	void SetUniform(std::string uniformName, glm::vec3 v) const;
	void SetUniform(std::string uniformName, glm::vec4 v) const;
	void SetUniform(std::string uniformName, float f) const;
	void SetUniform(std::string uniformName, unsigned int i) const;

private:
	unsigned int m_ShaderProgram;
	unsigned int m_VertexShader;
	unsigned int m_FragmentShader;

	void CreateVertexShader(std::string filepath);
	void CreateFragmentShader(std::string filepath);
	void CreateShaderProgram();

	std::string LoadShaderSource(std::string filepath) const;
};