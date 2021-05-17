#include "Window.h"
#include "GLFW/glfw3.h"

Window::Window(int width, int height, const char* name)
	: m_Width(width), m_Height(height), m_GLWindow(glfwCreateWindow(width, height, name, NULL, NULL))
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool Window::HasValidContext()
{
	return (m_GLWindow);
}

GLFWwindow* Window::GetWindow()
{
	return m_GLWindow;
}

void Window::Init()
{
	glfwMakeContextCurrent(m_GLWindow);
	glfwSwapInterval(1);
}

bool Window::ShouldClose()
{
	return (glfwWindowShouldClose(m_GLWindow));
}

void Window::OnClose()
{
	 glfwTerminate();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_GLWindow);
}

void Window::PollEvents()
{
	glfwPollEvents();
}
