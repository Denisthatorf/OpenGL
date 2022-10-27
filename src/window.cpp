#include "window.hpp"
#include <GLFW/glfw3.h>

bool window_create(window_state* state)
{
    if (!glfwInit()) 
        return false;

    state->window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!state->window)
        return false;

    return true;
}
void window_destroy(window_state* state)
{
    glfwDestroyWindow(state->window);
    glfwTerminate();
}

bool window_poolEvents(window_state* state)
{
    if(!glfwWindowShouldClose(state->window))
    {         
        glfwPollEvents();
        glfwSwapBuffers(state->window);
        return true;
    }
    return false;
}

void window_setContext(window_state* state)
{
    glfwMakeContextCurrent(state->window);
}