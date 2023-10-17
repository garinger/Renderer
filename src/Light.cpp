#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color)
	: m_Position(position), m_Color(color), m_Mesh("models/sphere.obj", glm::mat4(1.0f), "shaders/light.vert", "shaders/light.frag")
{

}

void Light::Update(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_Position);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 result = translate * scale;

	m_Mesh.SetModelMatrix(result);
	m_Mesh.Update(viewMatrix, projectionMatrix);

	m_Mesh.GetShader()->SetUniform("color", m_Color);
}