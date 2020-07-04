#include "VertexArray.h"
#include "Renderer.h"
VeretexArray::VeretexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VeretexArray::~VeretexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VeretexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset));
		offset += element.count * VertexBuferElement::GetSizeofType(element.type);

	}
}

void VeretexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VeretexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));

}
