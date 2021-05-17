#pragma once
#include <cstdint>

class VertexBuffer
{
private:
	uint32_t m_RendererID;
public:
	VertexBuffer(uint32_t size);
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void SetData(const void* data, uint32_t size);
};