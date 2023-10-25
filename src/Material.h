#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Material
{
public:
	Material(float specular, float diffuse, float ambient, float shininess);

	glm::vec4 GetMaterial() { return glm::vec4(m_Specular, m_Diffuse, m_Ambient, m_Shininess); };

private:
	float m_Specular;
	float m_Diffuse;
	float m_Ambient;
	float m_Shininess;
};