#include "Camera.h"

#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up,
    float aspectRatio, float fov, float nearPlane, float farPlane)
    : m_Position(position), m_Forward(forward), m_Up(up), m_Right(glm::vec3(0.0f)), m_View(),
    m_Projection(), m_AspectRatio(aspectRatio), m_Fov(fov), m_NearPlane(nearPlane), m_FarPlane(farPlane),
    m_IsPanning(false), m_IsLooking(false), m_ZoomSensitivity(1.0f), m_PanSensitivity(0.01f), m_LookSensitivity(0.2f)
{

}

void Camera::Update()
{
    // Every frame, update the matrices to align with where the camera is looking
    m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearPlane, m_FarPlane);

    // Calculate right vector
    m_Right = glm::normalize(glm::cross(m_Forward, m_Up));
}

void Camera::HandleInput(SDL_Event& event)
{
    if (event.type == SDL_MOUSEWHEEL)
    {
        // Check if the mouse wheel scrolled up or down
        float zoomInOrOut = event.wheel.y;

        // Translate the camera's position along its forward vector to simulate zooming in or out
        glm::vec3 movement = m_Forward * zoomInOrOut * m_ZoomSensitivity;
        m_Position += movement;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_MIDDLE)
        {
            m_IsPanning = true;
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {
            m_IsLooking = true;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_MIDDLE)
        {
            m_IsPanning = false;
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {
            m_IsLooking = false;
        }
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
        if (m_IsPanning)
        {
            int deltaX = event.motion.xrel;
            int deltaY = event.motion.yrel;
            Pan(deltaX, deltaY);
        } else if (m_IsLooking)
        {
            int deltaX = event.motion.xrel;
            int deltaY = event.motion.yrel;
            Look(deltaX, deltaY);
        }
    }
}

void Camera::Pan(int deltaX, int deltaY)
{
    float panX = deltaX * m_PanSensitivity;
    float panY = deltaY * m_PanSensitivity;

    // Translate the camera's position along its right vector according to the horizontal mouse movement
    m_Position = m_Position - m_Right * panX;

    // Translate the camera's position along its up vector according to the vertical mouse movement
    m_Position.y += m_Up.y * panY;
}

void Camera::Look(int deltaX, int deltaY)
{
    float lookX = deltaX * m_LookSensitivity;
    float lookY = deltaY * m_LookSensitivity;

    // Create a rotation matrix that will rotate a vector around the y axis according to the horizontal mouse movement
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-lookX), glm::vec3(0.0f, 1.0f, 0.0f));

    // TODO: Fix jittering when looking straight up and straight down
    // Multiply the rotation matrix with another rotation matrix that will rotate a vector around the camera's right vector.
    rotation = glm::rotate(rotation, glm::radians(-lookY), m_Right);

    // Multiply the camera's forward vector with this rotation matrix so we can change where the camera is looking
    glm::vec4 result = rotation * glm::vec4(m_Forward, 0.0f);

    m_Forward = glm::vec3(result);
}