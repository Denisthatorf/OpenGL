#include "renderer.hpp"
#include "opengl_types.hpp"

void Renderer::Clear() const
{
    GL_ASSERT(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    // Bind everything so we can draw
    shader.Bind();
    va.Bind();
    ib.Bind();
    
    // Draw the current selected buffer
    GL_ASSERT(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // nullptr, because the indices are bound to the current buffer: GL_ELEMENT_ARRAY_BUFFER
}