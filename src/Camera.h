#pragma once

#include "glad/glad.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, float aspectRatio, float fov, float nearPlane, float farPlane);

	void Update();

	void HandleInput(SDL_Event& event);

	glm::vec3 GetPosition() const { return m_Position; };
	glm::vec3 GetForward() const { return m_Forward; };
	glm::vec3 GetRight() const { return m_Right; };

	glm::mat4 GetViewMatrix() const { return m_View; };
	glm::mat4 GetProjectionMatrix() const { return m_Projection; };

private:
	glm::vec3 m_Position, m_Forward, m_Up, m_Right;
	glm::mat4 m_View, m_Projection;

	float m_AspectRatio, m_Fov, m_NearPlane, m_FarPlane;

	bool m_IsPanning, m_IsLooking;
	float m_ZoomSensitivity, m_PanSensitivity, m_LookSensitivity;

	void Pan(int deltaX, int deltaY);
	void Look(int deltaX, int deltaY);
};