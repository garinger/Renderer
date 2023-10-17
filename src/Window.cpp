#include "Window.h"

#include <iostream>

Window::Window(std::string title, int width, int height)
	: m_Title(title), m_Width(width), m_Height(height), m_AspectRatio((float)(m_Width) / (float)(m_Height)), m_ShouldClose(false), m_Focused(true)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "Failed to initialize SDL!\n" << SDL_GetError() << "\n";
		// Quit the program early, we can't go on without this
		std::terminate();
	}

	// Set OpenGL version (4.6) and make sure we're using OpenGL Core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Create our SDL window
	m_Window = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!m_Window)
	{
		std::cerr << "Failed to create SDL window!\n" << SDL_GetError() << "\n";
		// Again, quit the program early, we can't go on without this
		SDL_Quit();
		std::terminate();
	}

	// Create our OpenGL context
	m_Context = SDL_GL_CreateContext(m_Window);
	if (!m_Context)
	{
		std::cerr << "Failed to create OpenGL context!\n" << SDL_GetError() << "\n";
		// Again, quit the program early, we can't go on without this
		SDL_Quit();
		std::terminate();
	}

	// Setup GLAD so we can use OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!\n";
		// Last time... quit the program early, we can't go on without this
		SDL_Quit();
		std::terminate();
	}

	// Set OpenGL viewport to inital width & height
	ResizeWindow(m_Width, m_Height);

	// Enable depth testing so faces render in the correct Z order
	glEnable(GL_DEPTH_TEST);

	// Lock the cursor to the window
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::ResizeWindow(int width, int height)
{
	m_Width = width;
	m_Height = height;
	m_AspectRatio = (float)m_Width / (float)m_Height;

	// Let OpenGL know what our new aspect ratio is so the image isn't distorted
	glViewport(0, 0, m_Width, m_Height);
}

void Window::HandleInput(SDL_Event& event)
{
	// Check if we're trying to quit, return early
	if (event.type == SDL_QUIT || event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
	{
		m_ShouldClose = true;
		return;
	}

	if (event.type == SDL_KEYDOWN)
	{
		// Debug mode. SDLK_BACKQUOTE is `
		if (event.key.keysym.sym == SDLK_BACKQUOTE)
		{
			if (m_Focused)
			{
				// Release cursor from the window
				SDL_SetRelativeMouseMode(SDL_FALSE);
				m_Focused = false;
			}
			else
			{
				// Disregard events that occured while unfocused
				SDL_GetRelativeMouseState(nullptr, nullptr);

				// Lock cursor back into the window
				SDL_SetRelativeMouseMode(SDL_TRUE);
				m_Focused = true;
			}
		}
	}
	else if (event.type == SDL_WINDOWEVENT)
	{
		if (event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			// Make sure to update OpenGL viewport when we resize the window
			// Note: data1 is new width, data2 is new height
			ResizeWindow(event.window.data1, event.window.data2);
		}
	}
}

Window::~Window()
{
	// Cleanup SDL resources
	SDL_GL_DeleteContext(m_Context);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
	m_Window = nullptr;
}