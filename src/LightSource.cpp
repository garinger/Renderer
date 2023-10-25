#include "LightSource.h"

LightSource::LightSource(glm::vec3 position, glm::vec3 specular, glm::vec3 diffuse, glm::vec3 ambient)
	: m_Position(position), m_Specular(specular), m_Diffuse(diffuse), m_Ambient(ambient)
{

}