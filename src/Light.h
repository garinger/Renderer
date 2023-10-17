#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Mesh.h"

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 color);

	glm::vec3& GetPosition() { return m_Position; };
	glm::vec3& GetColor() { return m_Color; };

	void Update(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void TryDraw() { m_Mesh.TryDraw(); };

private:
	glm::vec3 m_Position;
	glm::vec3 m_Color;
	Mesh m_Mesh;
};
