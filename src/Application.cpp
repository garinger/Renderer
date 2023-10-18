#include "Application.h"

#include <iostream>
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

Application::Application()
	: m_Running(true), m_Window("Jackson's Renderer", 1280, 720), m_Clock(),
	m_Camera(glm::vec3(2.76f, 1.73f, 4.2f), glm::vec3(-0.52f, -0.3f, -0.8f), glm::vec3(0.0f, 1.0f, 0.0f),
	m_Window.GetAspectRatio(), 90.0f, 0.1f, 1000.0f), m_Meshes(), m_Lights(), m_Material({ 0.3f, 0.6f, 0.02f, 30.0f })
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
}

void Application::MainLoop()
{
	m_Meshes.push_back(std::make_unique<Mesh>(axisVertices, "shaders/axis.vert", "shaders/axis.frag", GL_LINES));
	m_Meshes.push_back(std::make_unique<Mesh>("models/monkey.obj", "textures/marble.jpg", "shaders/obj.vert", "shaders/obj.frag"));

	m_Lights.push_back(std::make_unique<Light>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f)));

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
	// ImGui menu
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
		// Create a collapsible section for each mesh
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			std::string name = "Mesh " + std::to_string(i);
			if (ImGui::TreeNode(name.c_str()))
			{
				ImGui::Checkbox("Visible", m_Meshes[i]->GetShouldDraw());
				ImGui::SliderFloat("Material Specular", &m_Material.specular, 0.0f, 1.0f, "%.2f");
				ImGui::SliderFloat("Material Diffuse", &m_Material.diffuse, 0.0f, 1.0f, "%.2f");
				ImGui::SliderFloat("Material Ambient", &m_Material.ambient, 0.0f, 1.0f, "%.2f");
				ImGui::SliderFloat("Material Shininess", &m_Material.shininess, 0.0f, 32.0f, "%.2f");
				ImGui::TreePop();
			}
		}
		// Create a collapsible section for each light
		for (int i = 0; i < m_Lights.size(); i++)
		{
			std::string name = "Light " + std::to_string(i);
			if (ImGui::TreeNode(name.c_str()))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(m_Lights[i]->GetPosition()), 0.01f, -100.0f, 100.0f, "%.2f");
				ImGui::DragFloat3("Specular", glm::value_ptr(m_Lights[i]->GetSpecular()), 0.01f, 0.0f, 1.0f, "%.2f");
				ImGui::DragFloat3("Diffuse", glm::value_ptr(m_Lights[i]->GetDiffuse()), 0.01f, 0.0f, 1.0f, "%.2f");
				ImGui::DragFloat3("Ambient", glm::value_ptr(m_Lights[i]->GetAmbient()), 0.01f, 0.0f, 1.0f, "%.2f");
				ImGui::TreePop();
			}
		}
	}

	// Update the camera's position and look direction
	m_Camera.Update();
	
	// TODO: Refactor this. Mesh's don't need to have their own shader.
	// Update the model, view, and projection matrices for the mesh's shader. Also, set the lights[] uniform.
	for (auto& mesh : m_Meshes)
	{

		mesh->Update(m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix());

		// TODO: Refactor this
		Shader* shader = mesh->GetShader();
		for (int i = 0; i < m_Lights.size(); i++)
		{
			shader->Use();

			std::string lightPos = "lights[" + std::to_string(i) + "].position";
			std::string lightSpecular = "lights[" + std::to_string(i) + "].specular";
			std::string lightDiffuse = "lights[" + std::to_string(i) + "].diffuse";
			std::string lightAmbient = "lights[" + std::to_string(i) + "].ambient";
			shader->SetUniform(lightPos, m_Lights[i]->GetPosition());
			shader->SetUniform(lightSpecular, m_Lights[i]->GetSpecular());
			shader->SetUniform(lightDiffuse, m_Lights[i]->GetDiffuse());
			shader->SetUniform(lightAmbient, m_Lights[i]->GetAmbient());

			shader->SetUniform("mat.specular", m_Material.specular);
			shader->SetUniform("mat.diffuse", m_Material.diffuse);
			shader->SetUniform("mat.ambient", m_Material.ambient);
			shader->SetUniform("mat.shininess", m_Material.shininess);

			shader->SetUniform("camera_look", m_Camera.GetPosition() + m_Camera.GetForward());
		}
	}
	
	// Update the model, view, and projection matrices for the lights' indictator sphere.
	for (auto& light : m_Lights)
	{
		light->Update(m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix());
	}
}

void Application::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw each mesh
	for (const auto& mesh : m_Meshes)
	{
		mesh->TryDraw();
	}

	// Draw the indicator sphere for each light
	for (const auto& light : m_Lights)
	{
		light->TryDraw();
	}

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