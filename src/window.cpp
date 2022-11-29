#include "opengl/opengl_types.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "window.hpp"

//TODO: think
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

Window::Window(int width, int height, const std::string& title)
{
    if (!glfwInit()){}
    //TODO: throw exeption

    m_glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    //TODO: throw exeption
    if(m_glfwWindow){}

    glfwSwapInterval(1);
}
Window::~Window()
{
    if(m_isImGuiUsed)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

bool Window::PoolEvents()
{
    if(!glfwWindowShouldClose(m_glfwWindow))
    {         
        glfwPollEvents();
        glfwSwapBuffers(m_glfwWindow);

        return true;
    }
    return false;
}

void Window::SetContext()
{
    //TODO: think about move to renderer
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwMakeContextCurrent(m_glfwWindow);
}

void Window::InitImGui()
{
    ImGui::CreateContext();      
    ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();
    m_isImGuiUsed = true;
}