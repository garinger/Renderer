#pragma once

#include "glm/glm.hpp"

class LightSource
{
public:
	LightSource(glm::vec3 position, glm::vec3 specular, glm::vec3 diffuse, glm::vec3 ambient);

	glm::vec3& GetPosition() { return m_Position; };
	glm::vec3& GetSpecular() { return m_Specular; };
	glm::vec3& GetDiffuse() { return m_Diffuse; };
	glm::vec3& GetAmbient() { return m_Ambient; };

private:
	glm::vec3 m_Position;
	glm::vec3 m_Specular;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Ambient;
};
