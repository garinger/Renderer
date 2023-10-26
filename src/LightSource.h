#pragma once

#include <memory>

#include "glm/glm.hpp"

#include "Transform.h"

class LightSource
{
public:
	LightSource(std::shared_ptr<Transform> transform, std::shared_ptr<bool> active, glm::vec3 specular, glm::vec3 diffuse, glm::vec3 ambient);

	const std::shared_ptr<Transform>& GetTransform() { return m_Transform; };
	const std::shared_ptr<bool>& GetActive() { return m_Active; };

	glm::vec3& GetSpecular() { return m_Specular; };
	glm::vec3& GetDiffuse() { return m_Diffuse; };
	glm::vec3& GetAmbient() { return m_Ambient; };

private:
	// This is a reference to the Transform from the parent SceneObject
	std::shared_ptr<Transform> m_Transform;

	// This is a reference to the Active bool from the parent SceneObject
	std::shared_ptr<bool> m_Active;

	glm::vec3 m_Position;
	glm::vec3 m_Specular;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Ambient;
};
