#include "Camera.h"

Camera::Camera(int x, int y)
	: m_Pos(x, y)
{
	m_View = (glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0)));
}

glm::vec2& Camera::GetPos()
{
	return m_Pos;
}

glm::mat4& Camera::GetView()
{
	return m_View;
}

void Camera::Move(glm::vec2 amount)
{
	m_Pos += amount;
	m_View = (glm::translate(glm::mat4(1.0f), glm::vec3(m_Pos, 0)));
}