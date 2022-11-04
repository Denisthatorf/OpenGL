#include "indexBuffer.hpp"
#include "opengl_types.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GL_ASSERT(glGenBuffers(1, &m_RendererID)); // Generate a single buffer
    GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // Select the buffer to be drawn
    GL_ASSERT(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); // Add the data to the buffer
}

IndexBuffer::~IndexBuffer()
{
    GL_ASSERT(glDeleteBuffers(1, &m_RendererID));  
}

void IndexBuffer::Bind() const
{
    GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
