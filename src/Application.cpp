#include "Application.h"

#include <iostream>
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

Application::Application()
	: m_Running(true), m_Window("Jackson's Renderer", 1280, 720), m_Clock(),
	m_Camera(glm::vec3(2.76f, 1.73f, 4.2f), glm::vec3(-0.52f, -0.3f, -0.8f), glm::vec3(0.0f, 1.0f, 0.0f),
	m_Window.GetAspectRatio(), 90.0f, 0.1f, 1000.0f), m_SceneObjects(), m_LightSources(), m_LightSourceData(), m_CameraUbo(0), m_LightSourcesUbo(0)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Dear ImGui styling
	{
		ImGui::GetStyle().WindowRounding = 1.0f;
		ImGui::GetStyle().FrameRounding = 2.0f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.13f, 0.13f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.64f, 0.64f, 0.64f, 0.35f);
	}

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_Window.GetWindow(), m_Window.GetContext());
	ImGui_ImplOpenGL3_Init();

	// Create the Uniform Buffer Object that will hold our camera information
	glGenBuffers(1, &m_CameraUbo);
	glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUbo);
	// We need to store 2 mat4s (view and projection matrices) and 1 vec3 (forward vector) 
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_CameraUbo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Create the Uniform Buffer Object that will hold all the lights in our scene
	glGenBuffers(1, &m_LightSourcesUbo);
	glBindBuffer(GL_UNIFORM_BUFFER, m_LightSourcesUbo);
	// Note: Since we're using the std140 standard in our UBO block binding, our shader is expecting our
	// vec3s to have the same padding as a vec4.
	// Each light has a vec3 for position, specular, diffuse, and ambient components.
	// 1 unsigned int to keep track of the number of active lights in the scene.
	glBufferData(GL_UNIFORM_BUFFER, 68, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_LightSourcesUbo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Application::MainLoop()
{
	// Monkey
	std::unique_ptr<SceneObject> object1 = std::make_unique<SceneObject>();
	std::unique_ptr<Transform> transform1 = std::make_unique<Transform>(glm::vec3(0.0f, 0.0f, 0.0f));
	std::unique_ptr<Mesh> mesh1 = std::make_unique<Mesh>("models/monkey.obj");
	std::unique_ptr<Shader> shader1 = std::make_unique<Shader>("shaders/phong.vert", "shaders/phong.frag");
	std::unique_ptr<Material> material1 = std::make_unique<Material>(0.3f, 0.5f, 0.02f, 32.0f);
	object1->SetTransform(transform1);
	object1->SetMesh(mesh1);
	object1->SetShader(shader1);
	object1->SetMaterial(material1);
	m_SceneObjects.push_back(std::move(object1));

	// Axis lines
	std::unique_ptr<SceneObject> object2 = std::make_unique<SceneObject>();
	std::unique_ptr<Transform> transform2 = std::make_unique<Transform>(glm::vec3(0.0f, 0.0f, 0.0f));
	std::unique_ptr<Mesh> mesh2 = std::make_unique<Mesh>(axisVertices, GL_LINES);
	std::unique_ptr<Shader> shader2= std::make_unique<Shader>("shaders/axis.vert", "shaders/axis.frag");
	object2->SetTransform(transform2);
	object2->SetMesh(mesh2);
	object2->SetShader(shader2);
	m_SceneObjects.push_back(std::move(object2));

	// Point light
	std::unique_ptr<SceneObject> object3 = std::make_unique<SceneObject>();
	std::unique_ptr<Transform> transform3 = std::make_unique<Transform>(glm::vec3(0.0f, 0.0f, 5.0f));
	std::unique_ptr<Mesh> mesh3 = std::make_unique<Mesh>("models/sphere.obj");
	std::unique_ptr<Shader> shader3 = std::make_unique<Shader>("shaders/light.vert", "shaders/light.frag");
	std::unique_ptr<LightSource> lightSource = std::make_unique<LightSource>(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	object3->SetTransform(transform3);
	object3->SetMesh(mesh3);
	object3->SetShader(shader3);
	object3->SetLightSource(lightSource);
	m_SceneObjects.push_back(std::move(object3));
	std::weak_ptr<LightSource> weakLight = m_SceneObjects[2]->GetLightSource();
	m_LightSources.push_back(weakLight);

	while (m_Running)
	{
		HandleInput();

		Update(m_Clock.GetFrameTime());

		Render();

		m_Clock.Tick();
	}
}

void Application::HandleInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// Handle input reated to the window
		m_Window.HandleInput(event);

		// See if the we tried to close the window
		m_Running = !m_Window.GetShouldClose();

		if (m_Window.GetFocused())
		{
			// Handle input reated to the camera
			m_Camera.HandleInput(event);
		}
		else
		{
			// Handle ImGui input
			ImGui_ImplSDL2_ProcessEvent(&event);
		}
	}
}

void Application::Update(double frameTime)
{
	// ImGui UI
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::SeparatorText("Stats");
		ImGui::Text("Frame Rate: %.2f", m_Clock.GetFrameRate());
		ImGui::Text("Recent Frame Rate: %.2f", m_Clock.GetRecentFrameRate());
		ImGui::Text("Frame Time: %.3f ms", m_Clock.GetFrameTime());
		ImGui::Text("Frame Count: %d", m_Clock.GetFrameCount());

		ImGui::SeparatorText("Scene");
		if (ImGui::TreeNode("Camera"))
		{
			glm::vec3 position = m_Camera.GetPosition();
			ImGui::Text("Position: %.2f, %.2f, %.2f", position.x, position.y, position.z);
			glm::vec3 forward = m_Camera.GetForward();
			ImGui::Text("Forward: %.2f, %.2f, %.2f", forward.x, forward.y, forward.z);
			glm::vec3 right = m_Camera.GetRight();
			ImGui::Text("Right: %.2f, %.2f, %.2f", right.x, right.y, right.z);
			ImGui::TreePop();
		}

		// Create a collapsible section for each SceneObject
		for (int i = 0; i < m_SceneObjects.size(); i++)
		{
			std::string name = "SceneObject " + std::to_string(i);
			if (ImGui::TreeNode(name.c_str()))
			{
				if (m_SceneObjects[i]->GetTransform())
				{
					ImGui::Text("Transform");
				}

				if (m_SceneObjects[i]->GetMesh())
				{
					ImGui::Text("Mesh");
				}

				if (m_SceneObjects[i]->GetMaterial())
				{
					ImGui::Text("Material");
				}

				if (m_SceneObjects[i]->GetShader())
				{
					ImGui::Text("Shader");
				}

				if (m_SceneObjects[i]->GetLightSource())
				{
					ImGui::Text("LightSource");
				}

				ImGui::TreePop();
			}
		}
	}

	// Update the camera's position and look direction
	m_Camera.Update();
	
	// Loop through all the lights in the scene
	m_LightSourceData.clear();
	for (std::weak_ptr<LightSource>& light : m_LightSources)
	{
		if (std::shared_ptr<LightSource> l = light.lock())
		{
			m_LightSourceData.push_back(glm::vec4(l->GetPosition(), 1.0f));
			m_LightSourceData.push_back(glm::vec4(l->GetSpecular(), 1.0f));
			m_LightSourceData.push_back(glm::vec4(l->GetDiffuse(), 1.0f));
			m_LightSourceData.push_back(glm::vec4(l->GetAmbient(), 1.0f));
		}
	}

	// Update all the SceneObjects in the scene
	for (std::unique_ptr<SceneObject>& object : m_SceneObjects)
	{
		object->Update();
	}

	// Update our Uniform Buffer Objects
	{
		// Update the matrices UBO
		glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUbo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_Camera.GetViewMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_Camera.GetProjectionMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(glm::vec4(m_Camera.GetForward(), 1.0f)));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Update the lights UBO
		glBindBuffer(GL_UNIFORM_BUFFER, m_LightSourcesUbo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(m_LightSourceData[0]));
		unsigned int num_active_lights = 1;
		glBufferSubData(GL_UNIFORM_BUFFER, 64, 4, &num_active_lights);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void Application::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Attempt to draw all the SceneObjects in the scene
	for (std::unique_ptr<SceneObject>& object : m_SceneObjects)
	{
		object->Draw();
	}

	// Render ImGui UI
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(m_Window.GetWindow());
}

Application::~Application()
{
	// ImGui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}