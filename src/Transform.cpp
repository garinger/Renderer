#include "Transform.h"

Transform::Transform(glm::vec3 position)
	: m_Position(position), m_Rotation(0.0f), m_Scale(1.0f)
{

}

glm::mat4 Transform::GetModelMatrix()
{
	glm::mat4 model(1.0f);
	glm::mat4 scale = glm::scale(model, m_Scale);
	glm::mat4 rotateX = glm::rotate(model, m_Rotation.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotateY = glm::rotate(model, m_Rotation.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotateZ = glm::rotate(model, m_Rotation.z, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 translate = glm::translate(model, m_Position);

	model *= scale * rotateX * rotateY * rotateZ * translate;
	return model;
}