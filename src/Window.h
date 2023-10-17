#pragma once

#include <string>

#include "glad/glad.h"
#include "SDL.h"
#include "SDL_opengl.h"

class Window
{
public:
	Window(std::string title, int width, int height);
	~Window();

	void HandleInput(SDL_Event& event);

	SDL_Window* GetWindow() const { return m_Window; };
	SDL_GLContext* GetContext() { return &m_Context; };

	float GetAspectRatio() const { return (float)m_Width / (float)m_Height; };

	bool GetShouldClose() const { return m_ShouldClose; };
	bool GetFocused() const { return m_Focused; };

private:
	void ResizeWindow(int width, int height);

	SDL_Window* m_Window;
	SDL_GLContext m_Context;

	std::string m_Title;
	int m_Width;
	int m_Height;
	float m_AspectRatio;

	bool m_ShouldClose;
	bool m_Focused;
};