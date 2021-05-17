#pragma once

#include "GLErrorManager.h"
#include <string>

class Texture
{
private:
	uint32_t m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(uint32_t width, uint32_t height);
	Texture(const std::string& path);
	~Texture();

	void Bind(uint32_t slot = 0) const;
	void Unbind() const;

	void SetData(void* data, uint32_t size);

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};