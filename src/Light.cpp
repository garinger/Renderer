#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 specular, glm::vec3 diffuse, glm::vec3 ambient)
	: m_Position(position), m_Specular(specular), m_Diffuse(diffuse), m_Ambient(ambient),
	m_Mesh("models/sphere.obj", glm::mat4(1.0f), "shaders/light.vert", "shaders/light.frag")
{

}

void Light::Update(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_Position);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 result = translate * scale;

	m_Mesh.SetModelMatrix(result);
	m_Mesh.Update(viewMatrix, projectionMatrix);

	m_Mesh.GetShader()->SetUniform("color", m_Ambient);
}