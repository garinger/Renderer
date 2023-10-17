#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

static std::vector<float> cubeVertices = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

 static std::vector<float> axisVertices = {
	 -100.0, 0.0, 0.0,  0.0, 0.0,   0.0, 0.0, 0.0,
	  100.0, 0.0, 0.0,  0.0, 0.0,   0.0, 0.0, 0.0,
	  0.0, 0.0, -100.0, 0.0, 0.0,   0.0, 0.0, 0.0,
	  0.0, 0.0, 100.0,  0.0, 0.0,   0.0, 0.0, 0.0,
 };

class Mesh
{
public:
    Mesh(std::vector<float> vertices, std::string vertShaderFilePath, std::string fragShaderFilePath, unsigned int drawMode);
    Mesh(std::string modelFilePath, glm::mat4 modelMatrix, std::string vertShaderFilePath, std::string fragShaderFilePath);
    Mesh(std::string modelFilePath, std::string textureFilePath, std::string vertShaderFilePath, std::string fragShaderFilePath);

	glm::mat4 GetModelMatrix() const { return m_ModelMatrix; };
    Shader* GetShader() { return &m_Shader; };
    bool* GetShouldDraw() { return &m_ShouldDraw; };

	void SetModelMatrix(glm::mat4 m) { m_ModelMatrix = m; };

    void Update(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void TryDraw() const;

private:
	void CreateBuffers();

    void LoadObjModel(std::string filepath);
    void LoadTexture(std::string filepath);

	std::vector<float> m_Vertices;
	unsigned int m_Vao;
	unsigned int m_Vbo;
	unsigned int m_Texture;
    unsigned int m_DrawMode;

	Shader m_Shader;
	glm::mat4 m_ModelMatrix;

    bool m_ShouldDraw;
};
