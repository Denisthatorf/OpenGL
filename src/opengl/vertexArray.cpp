#include "vertexArray.hpp"
#include "vertexBufferLayout.hpp"
#include "opengl_types.hpp"

VertexArray::VertexArray()
{
    GL_ASSERT(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GL_ASSERT(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
{
    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GL_ASSERT(glEnableVertexAttribArray(i));
        GL_ASSERT(glVertexAttribPointer(i, element.count, element.type, 
                element.normalized, layout.GetStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GL_ASSERT(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GL_ASSERT(glBindVertexArray(0));
}
