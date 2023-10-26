#include "LightSource.h"

LightSource::LightSource(std::shared_ptr<Transform> transform, std::shared_ptr<bool> active, glm::vec3 specular, glm::vec3 diffuse, glm::vec3 ambient)
	: m_Transform(transform), m_Active(active), m_Specular(specular), m_Diffuse(diffuse), m_Ambient(ambient)
{

}