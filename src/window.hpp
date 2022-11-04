#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool PoolEvents();
    void SetContext();
    void SwapBuffers();

    void InitImGui();

private:
    GLFWwindow* m_glfwWindow;
    bool m_isImGuiUsed;

    const char* glsl_version = "#version 130";
};