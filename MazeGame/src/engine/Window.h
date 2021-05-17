#pragma once
#include "GLFW/glfw3.h"
class Window
{
private:
	int m_Width, m_Height;
	GLFWwindow* m_GLWindow;
public:
	Window(int width, int height, const char* name);
	~Window();

	void OnClose();
	void SwapBuffers();
	void PollEvents();
	bool HasValidContext();
	bool ShouldClose();
	void Init();
	GLFWwindow* GetWindow();
};