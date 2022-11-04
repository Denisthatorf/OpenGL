#pragma once

#include <GL/glew.h>

#include "vertexArray.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};