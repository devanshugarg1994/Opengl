#pragma once

class VeretexArray
{
private:

public : 
	VeretexArray();
	~VeretexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};