#include "opengl/opengl_types.hpp"
#include "window.hpp"

Window::Window(int width, int height, const std::string& title)
{
    if (!glfwInit()){}
    //TODO: throw exeption

    m_glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if(m_glfwWindow){}
    //TODO: throw exeption
}
Window::~Window()
{
    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

bool Window::poolEvents()
{
    if(!glfwWindowShouldClose(m_glfwWindow))
    {         
        glfwPollEvents();
        glfwSwapBuffers(m_glfwWindow);
        return true;
    }
    return false;
}

void Window::setContext()
{
    //TODO: think about move to renderer
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwMakeContextCurrent(m_glfwWindow);
}