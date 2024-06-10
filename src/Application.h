#pragma once

#include <memory>

#include "Window.h"
#include "Clock.h"

#include "Camera.h"
#include "SceneObject.h"

class Application
{
public:
	Application();
	~Application();

	void MainLoop();

private:
	void HandleInput();
	void Update(const double frameTime);
	void Render();

	bool m_Running;
	Window m_Window;
	Clock m_Clock;

	Camera m_Camera;
	std::vector<std::unique_ptr<SceneObject>> m_SceneObjects;
	std::vector<std::weak_ptr<LightSource>> m_LightSources;
	std::vector<glm::vec4> m_LightSourceData;

	// TODO: Maybe refactor these
	unsigned int m_CameraUbo;
	unsigned int m_LightSourcesUbo;

	// TODO: Refactor this
	unsigned int m_MaxNumLights;
	unsigned int m_NumActiveLights;
};