#include "opengl_types.hpp"
#include "logger.hpp"

void glClearError()
{ 
    while(glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* file, int line, const char* function)
{
    while(GLenum error = glGetError())
    {
        //TODO: %i in hex
       log_warn("[OpenGL Error] (%i): file %s, line %i, funciton %s", error, file, line, function);
       return false;
    }
    return true;
}