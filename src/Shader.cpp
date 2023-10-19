#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(std::string vertexPath, std::string fragmentPath)
	: m_ShaderProgram(0), m_VertexShader(0), m_FragmentShader(0)
{
	// Create a vertex shader from a glsl file
	CreateVertexShader(vertexPath);

	// Create a fragent shader from a glsl file
	CreateFragmentShader(fragmentPath);

	// Once the vertex shader and fragment shader are loaded, 
	// we can create the shader program
	CreateShaderProgram();
}

void Shader::CreateVertexShader(std::string filepath)
{
	// Used to store compile and link status
	int success = 0;
	char infoLog[512]{};

	// Read the shader code from our source file
	std::string source = LoadShaderSource(filepath);
	const char* shaderCode = source.c_str();

	// Create and compile the shader
	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexShader, 1, &shaderCode, nullptr);
	glCompileShader(m_VertexShader);

	// Check to see if the shader compiled successfully
	glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_VertexShader, 512, NULL, infoLog);
		std::cerr << "Failed to compile vertex shader!\n" << infoLog << "\n";
	}
}

void Shader::CreateFragmentShader(std::string filepath)
{
	// Used to store compile and link status
	int success = 0;
	char infoLog[512]{};

	// Read the shader code from our source file
	std::string source = LoadShaderSource(filepath);
	const char* shaderCode = source.c_str();

	// Create and compile the shader
	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentShader, 1, &shaderCode, nullptr);
	glCompileShader(m_FragmentShader);

	// Check to see if the shader compiled successfully
	glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_FragmentShader, 512, NULL, infoLog);
		std::cerr << "Failed to compile fragment shader!\n" << infoLog << "\n";
	}
}

void Shader::CreateShaderProgram()
{
	// Used to store compile and link status
	int success = 0;
	char infoLog[512]{};

	// Create the shader program
	m_ShaderProgram = glCreateProgram();

	// Attach and link the vertex and fragment shaders to the shader program
	glAttachShader(m_ShaderProgram, m_VertexShader);
	glAttachShader(m_ShaderProgram, m_FragmentShader);
	glLinkProgram(m_ShaderProgram);

	// Check to see if the shader program linked successfully
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		std::cerr << "Failed to link shader program!\n" << infoLog << "\n";
	}

	// Once we link the shaders to the shader program, we can don't need
	// the shaders anymore
	glDetachShader(m_ShaderProgram, m_VertexShader);
	glDetachShader(m_ShaderProgram, m_FragmentShader);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
}

std::string Shader::LoadShaderSource(std::string filepath) const
{
	// Open the source file
	std::ifstream file(filepath);
	std::stringstream buffer;

	// Read the shader source code into the buffer
	buffer << file.rdbuf();

	return buffer.str();
}

void Shader::SetUniform(std::string uniformName, glm::mat4 m) const
{
	Use();
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::SetUniform(std::string uniformName, glm::vec3 v) const
{
	Use();
	glUniform3fv(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()), 1, glm::value_ptr(v));
}

void Shader::SetUniform(std::string uniformName, glm::vec4 v) const
{
	Use();
	glUniform4fv(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()), 1, glm::value_ptr(v));
}

void Shader::SetUniform(std::string uniformName, float f) const
{
	Use();
	glUniform1f(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()), f);
}

void Shader::SetUniform(std::string uniformName, unsigned int i) const
{
	Use();
	glUniform1ui(glGetUniformLocation(m_ShaderProgram, uniformName.c_str()), i);
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgram);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
}