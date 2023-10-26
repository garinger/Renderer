#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Transform
{
public:
	Transform(glm::vec3 position);

	glm::vec3& GetPosition() { return m_Position; };

	glm::mat4 GetModelMatrix();

private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
};