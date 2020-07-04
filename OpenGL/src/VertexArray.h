#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
class VeretexArray
{
private:
	unsigned int m_RendererID;
public : 
	VeretexArray();
	~VeretexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void UnBind() const;
};