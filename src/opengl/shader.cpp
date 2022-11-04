#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "opengl_types.hpp"

#include "shader.hpp"

Shader::Shader(const std::string & filePath)
    : m_FilePath(filePath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filePath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GL_ASSERT(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string filePath)
{
    std::ifstream stream(filePath);

    enum class Shadertype
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    Shadertype type = Shadertype::NONE;

    while (getline(stream, line))
    {
        // Set the type if #shader is found
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = Shadertype::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = Shadertype::FRAGMENT;
        }
        else // Add the line to the source
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[(int)Shadertype::VERTEX].str(), ss[(int)Shadertype::FRAGMENT].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); // Create the shader
    const char* src = source.c_str(); // Return the pointer of the first character of the source
    GL_ASSERT(glShaderSource(id, 1, &src, nullptr)); // Specify the shader source code
    GL_ASSERT(glCompileShader(id));

    int result;
    GL_ASSERT(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // Returns the compile status parameter

    if (result == GL_FALSE)
    {
        int length;
        GL_ASSERT(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char)); // Allocate this on the stack dynamically because 'char message[length]' is not allowed
        GL_ASSERT(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << std::endl;
        std::cout << message << std::endl;
        GL_ASSERT(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); // Create a shader program to attach shader to
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach both shaders to the program
    GL_ASSERT(glAttachShader(program, vs));
    GL_ASSERT(glAttachShader(program, fs));

    GL_ASSERT(glLinkProgram(program)); // Link the program so the shaders are used
    GL_ASSERT(glValidateProgram(program)); // Check if the program can be executed

    GL_ASSERT(glDeleteShader(vs));
    GL_ASSERT(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GL_ASSERT(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GL_ASSERT(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GL_ASSERT(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GL_ASSERT(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
    GL_ASSERT(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GL_ASSERT(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        //TODO: log
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location;
}
