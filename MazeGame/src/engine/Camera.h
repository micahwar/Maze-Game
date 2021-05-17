#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Camera
{
private:
	glm::mat4 m_View;
public:
	glm::vec2 m_Pos;
	Camera(int x, int y);
	~Camera() {}

	glm::vec2& GetPos();
	glm::mat4& GetView();
	void Move(glm::vec2 amount);
};