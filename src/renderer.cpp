#include "renderer.hpp"
#include "renderer.hpp"

#include <GL/glew.h>
#include "window.hpp"
#include "logger.hpp"

#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>
#include <string>

//TODO: move to define.hpp
//TODO: make debug
#include <csignal>
#define ASSERT(x) if(!(x)) std::raise(SIGINT)

#define GL_ASSERT(x)                \
    glClearError();                 \
    x;                              \
    ASSERT(glLogCall(__FILE__, __LINE__, #x))            \

//TODO: move
void glClearError()
{ 
    while(glGetError() != GL_NO_ERROR);
}
bool glLogCall(const char* file, int line, const char* function)
{
    while(GLenum error = glGetError())
    {
       log_warn("[OpenGL Error] (%i): file %s, line %i, funciton %s", error, file, line, function);
       return false;
    }
    return true;
}

float position[] = 
{
    -0.5f, -0.5f, // 0
     0.5f, -0.5f, // 1
     0.5f,  0.5f, // 2
    -0.5f,  0.5f  // 3
};

unsigned int indexes[] = 
{
    0,1,2,
    2,3,0
};

//TODO: think
struct ShaderProgramSource
{
	std::string fragmentShader;
	std::string vertexShader;
};

int shader_create(const std::string& vertexShader, const std::string& fragmentShader);
int shader_compile(unsigned int type, const std::string& source);
ShaderProgramSource shader_parse(const std::string& filepath);

bool renderer_init(window_state* state)
{
    window_setContext(state);
    //TODO: think
    glfwSwapInterval(1);

    glewExperimental = GL_FALSE;
    unsigned int error = glGetError();

    if (error != GL_NO_ERROR)
        return false;

    unsigned int glewinit = glewInit();
    if (glewinit != GLEW_OK) 
        return false;

    //TODO: remove this
    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), position, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    unsigned int ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    //TODO: make relative
	ShaderProgramSource shader_source = shader_parse("/home/denis/Projects/C++/OpenGL/shaders/red_triangle.shader");	

    unsigned int shader = shader_create(shader_source.vertexShader, shader_source.fragmentShader);
    glUseProgram(shader);

    //TODO: think
    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != (-1));
    GL_ASSERT(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    return true;
}

bool render_test_draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    GL_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    return true;
}

int shader_compile(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int err;
    glGetShaderiv(id, GL_COMPILE_STATUS, &err);

    if(err == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        //TODO: logger
        std::cout << "Failed to compile"  
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader!\n";
        std::cout << message << "\n";
        glDeleteShader(id);
        return 0;
    }

    return id;
}

int shader_create(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

    int vs = shader_compile(GL_VERTEX_SHADER, vertexShader);
    int fs = shader_compile(GL_FRAGMENT_SHADER, fragmentShader);

    //TODO: check if vs or fs == 0

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;  
}

//TODO: use C FILE*
//TODO: use char*
//TODO: clean code
#include <fstream>
#include <string>
#include <sstream>

enum class ShaderType
{ NONE = -1, VERTEX = 0, FRAGMENT = 1 };

ShaderProgramSource shader_parse(const std::string& filepath)
{
	std::ifstream file(filepath);

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while(getline(file, line))
	{
		if(line.find("#shader") != std::string::npos)
		{
			if(line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if(line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}

		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}
