#pragma once

//TODO: remove glew.h
#include <GL/glew.h>
#include <GL/gl.h>

#include <csignal>

#define ASSERT(x) if(!(x)) std::raise(SIGINT)

#define GL_ASSERT(x)                \
    glClearError();                 \
    x;                              \
    ASSERT(glLogCall(__FILE__, __LINE__, #x))            \

//TODO: move
void glClearError();
bool glLogCall(const char* file, int line, const char* function);