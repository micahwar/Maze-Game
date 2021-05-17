#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <memory>
#include <functional>

class Renderer2d
{
private:
	
	Renderer2d() {}
public:

	static int InitContext(int w, int h);
	static int InitWindow(int w, int h);

	static void DrawLoop(std::function<void()> drawGame);

	static void StartBatch();
	static void Flush();
	static void NextBatch();

	static void Background(int rgb);
	static void Background(int r, int g, int b);
	static void Background(Texture* texture);
	static void SetDrawColor(int rgb);
	static void SetDrawColor(int r, int g, int b);
	
	static void Rect(glm::vec2& position, glm::vec2& size);
	static void Rect(glm::vec2& position, glm::vec2& size, Texture* texture);

	static void Rect(glm::vec2& position, glm::vec2& size, Texture* texture, glm::vec4 color);
	
	static glm::vec2& GetWindowSize();
	static Camera* GetCamera();

	static Texture* LoadImage(std::string& filepath);
private:
	static float NormaliseRGB(int c);
};