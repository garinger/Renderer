#pragma once

#include <vector>
#include <string>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

 static std::vector<float> axisVertices = {
	 -100.0, 0.0, 0.0,  0.0, 0.0,   0.0, 0.0, 0.0,
	  100.0, 0.0, 0.0,  0.0, 0.0,   0.0, 0.0, 0.0,
	  0.0, 0.0, -100.0, 0.0, 0.0,   0.0, 0.0, 0.0,
	  0.0, 0.0, 100.0,  0.0, 0.0,   0.0, 0.0, 0.0,
 };

class Mesh
{
public:
	Mesh(std::string modelFilePath);
    Mesh(std::vector<float> vertices, unsigned int drawMode);
	~Mesh();

	void Draw() const;

private:
	void CreateBuffers();

    void LoadObjModel(std::string filepath);

	std::vector<float> m_Vertices;
	unsigned int m_Vao;
	unsigned int m_Vbo;
    unsigned int m_DrawMode;
};
