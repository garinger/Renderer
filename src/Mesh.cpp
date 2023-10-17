#include "Mesh.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "glad/glad.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "stb_image.h"

Mesh::Mesh(std::vector<float> vertices, std::string vertShaderFilePath, std::string fragShaderFilePath, unsigned int drawMode)
	: m_Vertices(vertices), m_Vao(), m_Vbo(), m_Texture(0), m_DrawMode(drawMode),
	m_Shader(vertShaderFilePath, fragShaderFilePath), m_ModelMatrix(glm::mat4(1.0f)), m_ShouldDraw(true)
{
	CreateBuffers();
}

Mesh::Mesh(std::string modelFilePath, glm::mat4 modelMatrix, std::string vertShaderFilePath, std::string fragShaderFilePath)
	: m_Vertices(), m_Vao(), m_Vbo(), m_Texture(0), m_DrawMode(GL_TRIANGLES),
	m_Shader(vertShaderFilePath, fragShaderFilePath), m_ModelMatrix(modelMatrix), m_ShouldDraw(true)
{
	LoadObjModel(modelFilePath);
	CreateBuffers();
}

Mesh::Mesh(std::string modelFilePath, std::string textureFilePath, std::string vertShaderFilePath, std::string fragShaderFilePath)
	: m_Vertices(), m_Vao(), m_Vbo(), m_Texture(0), m_DrawMode(GL_TRIANGLES),
	m_Shader(vertShaderFilePath, fragShaderFilePath), m_ModelMatrix(glm::mat4(1.0f)), m_ShouldDraw(true)
{
	LoadObjModel(modelFilePath);
	LoadTexture(textureFilePath);
	CreateBuffers();
}

void Mesh::CreateBuffers()
{
	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	glGenBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);

	// Texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 5));
	glEnableVertexAttribArray(2);
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

void Mesh::LoadObjModel(std::string filepath)
{
	// Typically, each face is encoded by 3 ' ' seperated vertices that contain 3 offset indices.
	// For example, "f 45/45/3 5/5/3 43/43/3".
	// The format of the vertices is typically position/texture/normal.
	// Each number is 1-base index into its corresponding array. So for the above example,
	// 45/45/3 encodes a vertex that has values from positions[44], textures[44], and normals[2].
	// 
	// Note: It seems like there are serveral variations of the Wavefront .obj format. Some use quads instead of tris,
	// some use negative offset indices for faces (meaning offset from the end), and some don't have texture or normal vertices.
	// 
	// Currently, this implementation assumes that each face is a triangle, and each face is encoded with positive offsets
	// for position, texture, and normal vertices.

	std::ifstream file(filepath);

	std::vector<std::vector<float>> positions;
	std::vector<std::vector<float>> normals;
	std::vector<std::vector<float>> textures;

	if (file.is_open())
	{
		// Get each line of the obj file
		std::string line;
		while (std::getline(file, line))
		{
			// Split the line into tokens
			std::vector<std::string> tokens = split(line, ' ');
			if (tokens.empty()) continue;

			// Remove any empty tokens
			std::vector<std::string>::iterator it = tokens.begin();
			while ((it = std::find(tokens.begin(), tokens.end(), "")) != tokens.end())
			{
				tokens.erase(it);
			}

			// If the line starts with "vn", we know it's a normal vertex
			if (tokens[0] == "vn")
			{
				normals.push_back({ std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) });
			}
			// If the line starts with "vt", we know it's a UV vertex
			else if (tokens[0] == "vt")
			{
				textures.push_back({ std::stof(tokens[1]), std::stof(tokens[2]) });
			}
			// If the line starts with "v", we know it's a position vertex
			else if (tokens[0] == "v")
			{
				positions.push_back({ std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) });
			}
			// If the line starts with "f", we know it's a face
			else if (tokens[0] == "f")
			{
				// Split the tokens into the 3 triangle vertices
				std::vector<std::vector<std::string>> vertices;
				vertices.push_back(split(tokens[1], '/'));
				vertices.push_back(split(tokens[2], '/'));
				vertices.push_back(split(tokens[3], '/'));


				// For each vertex, get the offset into each array and push the data
				// into m_Vertices which will eventually be sent to the VBO so it can
				// be rendered by the GPU
				for (const auto& vertex : vertices)
				{
					int positionIndex = stoi(vertex[0]) - 1;
					int textureIndex = stoi(vertex[1]) - 1;
					int normalIndex = stoi(vertex[2]) - 1;

					for (const float& component : positions[positionIndex])
					{
						m_Vertices.push_back(component);
					}

					for (const float& component : textures[textureIndex])
					{
						m_Vertices.push_back(component);
					}

					for (const float& component : normals[normalIndex])
					{
						m_Vertices.push_back(component);
					}
				}
			}
		}
	}

	file.close();
}

void Mesh::LoadTexture(std::string filepath)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Mesh::Update(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	m_Shader.Use();
	m_Shader.SetUniform("model", m_ModelMatrix);
	m_Shader.SetUniform("view", viewMatrix);
	m_Shader.SetUniform("projection", projectionMatrix);
}

void Mesh::TryDraw() const
{
	if (!m_ShouldDraw) return;

	m_Shader.Use();
	glBindVertexArray(m_Vao);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glDrawArrays(m_DrawMode, 0, m_Vertices.size() / 8);
}
