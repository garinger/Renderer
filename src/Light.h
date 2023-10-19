#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Mesh.h"

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 specular, glm::vec3 diffuse, glm::vec3 ambient);

	glm::vec3& GetPosition() { return m_Position; };
	glm::vec3& GetSpecular() { return m_Specular; };
	glm::vec3& GetDiffuse() { return m_Diffuse; };
	glm::vec3& GetAmbient() { return m_Ambient; };
	bool* GetIsActive() { return &m_Active; };

	void Update(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void TryDraw() { if (m_Active) m_Mesh.TryDraw(); };

private:
	glm::vec3 m_Position;
	glm::vec3 m_Specular;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Ambient;

	bool m_Active;

	Mesh m_Mesh;
};
