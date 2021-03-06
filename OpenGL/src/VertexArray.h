#pragma once

#include "VertexBuffer.h"
class VertexBufferLayout;


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