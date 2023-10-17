#pragma once

#include <memory>

#include "Window.h"
#include "Clock.h"

#include "Camera.h"
#include "Mesh.h"
#include "Light.h"

class Application
{
public:
	Application();
	~Application();

	void MainLoop();

private:
	void HandleInput();
	void Update(double frameTime);
	void Render();

	bool m_Running;
	Window m_Window;
	Clock m_Clock;

	Camera m_Camera;
	std::vector<std::unique_ptr<Mesh>> m_Meshes;
	std::vector<std::unique_ptr<Light>> m_Lights;
};