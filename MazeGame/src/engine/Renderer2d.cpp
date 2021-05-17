#include "Renderer2d.h"
#include "GLErrorManager.h"
#include <iostream>
#include <array>

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
};

struct Renderer2DData
{
	static const uint32_t MaxQuads = 1000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;

	VertexArray* QuadVertexArray;
	VertexBuffer* QuadVertexBuffer;
	IndexBuffer* QuadIndexBuffer;
	Shader* TextureShader;
	Texture* WhiteTexture;

	uint32_t QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	std::array<Texture*, 32> TextureSlots;

	glm::vec4 QuadVertexPositions[4];

	glm::vec2 WindowSize;
	GLFWwindow* GLFWwindow;
	Camera* Camera;

	glm::mat4 Projection;
	glm::vec4 CurrentColor;
};

static Renderer2DData s_Data;

int Renderer2d::InitContext(int w, int h)
{	
	if (!InitWindow(w, h))
		return 0; //my dumbass forgot to put this first so the window wouldn't load at all and got stuck on new VertexArray()
	//my dumbass also forgot he had to recode the shader file

	std::cout << "Initialised 0.0f" << std::endl;
	s_Data.QuadVertexArray = new VertexArray();
	std::cout << "Initialised 0" << std::endl;
	s_Data.QuadVertexBuffer = new VertexBuffer(s_Data.MaxVertices * sizeof(QuadVertex));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);
	layout.Push<float>(2);
	layout.Push<float>(1);
	s_Data.QuadVertexArray->AddBuffer(*(s_Data.QuadVertexBuffer), layout);
	std::cout << "Initialised 1" << std::endl;

	s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

	uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}
	std::cout << "Initialised 2" << std::endl;

	s_Data.QuadIndexBuffer = new IndexBuffer(quadIndices, s_Data.MaxIndices);

	s_Data.WhiteTexture = new Texture(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	
	std::cout << "Initialised 3" << std::endl;

	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	s_Data.WindowSize = glm::vec2(w, h);
	s_Data.Camera = new Camera(0, 0);
	s_Data.Projection = glm::ortho(0.0f, (float)w, 0.0f, float(h), -1.0f, 1.0f);
	std::cout << "Initialised 4" << std::endl;

	
	std::cout << "Initialised 5" << std::endl;

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	s_Data.TextureShader = new Shader("res/shaders/Textured.glsl");
	std::cout << "Initialised 6" << std::endl;

	return 1;
}

int Renderer2d::InitWindow(int w, int h)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!glfwInit())
		return 0;

	s_Data.GLFWwindow = glfwCreateWindow(w, h, "Maze Game", NULL, NULL);

	if (!s_Data.GLFWwindow)
	{
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(s_Data.GLFWwindow);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		return 0;

	return 1;
}

Texture* Renderer2d::LoadImage(std::string& filepath)
{
	return new Texture(filepath);
}

void Renderer2d::DrawLoop(std::function<void()> drawGame)
{
	//KeyEvent keyEvent = KeyEvent(window->GetWindow());
	double lasttime = glfwGetTime();
	while (!glfwWindowShouldClose(s_Data.GLFWwindow))
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		StartBatch();
		drawGame();
		Flush();
		glfwSwapBuffers(s_Data.GLFWwindow);
		glfwPollEvents();
		while (glfwGetTime() < lasttime + 1.0 / 60) {
			// game intensly
		}
		lasttime += 1.0 / 60;
	}
	glfwTerminate();
}

void Renderer2d::StartBatch()
{
	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
}

void Renderer2d::Flush()
{
	if (s_Data.QuadIndexCount == 0)
		return;
	uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
	
	s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
	
	s_Data.WhiteTexture->Bind(0);
	s_Data.TextureShader->Bind();
	glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer2d::NextBatch()
{
	Flush();
	StartBatch();
}

	
void Renderer2d::Rect(glm::vec2& position, glm::vec2& size)
{
	Renderer2d::Rect(position, size, s_Data.WhiteTexture, s_Data.CurrentColor);
}

void Renderer2d::Rect(glm::vec2& position, glm::vec2& size, Texture* texture)
{
	Renderer2d::Rect(position, size, texture, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void Renderer2d::Rect(glm::vec2& position, glm::vec2& size, Texture* texture, glm::vec4 color)
{
	constexpr size_t quadVertexCount = 4;
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		Renderer2d::NextBatch();

	glm::vec3 position3 = glm::vec3(position, 0.0f);
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position3) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = s_Data.Projection * s_Data.Camera->GetView() * transform * s_Data.QuadVertexPositions[i];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		s_Data.QuadVertexBufferPtr->TexIndex = 0.0f;
		s_Data.QuadVertexBufferPtr++;
	}
	s_Data.QuadIndexCount += 6;
}

glm::vec2& Renderer2d::GetWindowSize()
{
	return s_Data.WindowSize;
}

Camera* Renderer2d::GetCamera()
{
	return s_Data.Camera;
}

float Renderer2d::NormaliseRGB(int c)
{
	return 0.0f + ((1.0f - 0.0f) / (255.0f - 0.0f)) * (c - 0.0f);
}

void Renderer2d::Background(int rgb)
{
	float nrgb = NormaliseRGB(rgb);
	GLCall(glClearColor(nrgb, nrgb, nrgb, 1.0f));
}

void Renderer2d::Background(Texture* texture)
{
	Rect(s_Data.Camera->GetPos(), s_Data.WindowSize, texture);
}

void Renderer2d::Background(int r, int g, int b)
{
	GLCall(glClearColor(NormaliseRGB(r), NormaliseRGB(g), NormaliseRGB(b), 1.0f));
}

void Renderer2d::SetDrawColor(int rgb)
{
	float nrgb = NormaliseRGB(rgb);
	s_Data.CurrentColor = glm::vec4(nrgb, nrgb, nrgb, 1.0f);
}

void Renderer2d::SetDrawColor(int r, int g, int b)
{
	s_Data.CurrentColor = glm::vec4(NormaliseRGB(r), NormaliseRGB(g), NormaliseRGB(b), 1.0f);
}