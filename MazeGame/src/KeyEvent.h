#pragma once
#include "GLFW/glfw3.h"
class KeyEvent
{
private:
	int m_Prev, m_Curr;
	GLFWwindow* m_Window;
public:
	KeyEvent(GLFWwindow* window);
	~KeyEvent() {}

	bool SpacePressed();
};

KeyEvent::KeyEvent(GLFWwindow* window)
	: m_Window(window), m_Prev(0), m_Curr(glfwGetKey(window, GLFW_KEY_SPACE))
{
}

bool KeyEvent::SpacePressed()
{
	m_Prev = m_Curr;
	m_Curr = glfwGetKey(m_Window, GLFW_KEY_SPACE);;
	if (m_Prev != m_Curr && m_Curr == GLFW_PRESS)
	{
		return 1;
	}
	return 0;
}