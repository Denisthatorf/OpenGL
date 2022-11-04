#include "vertexBuffer.hpp"
#include "opengl_types.hpp"

VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
    GL_ASSERT(glGenBuffers(1, &m_RendererID)); // Generate a single buffer
    GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Select the buffer to be drawn
    GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // Add the data to the buffer
}

VertexBuffer::~VertexBuffer()
{
    GL_ASSERT(glDeleteBuffers(1, &m_RendererID));  
}

void VertexBuffer::Bind() const
{
    GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
