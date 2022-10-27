#include <GLFW/glfw3.h>

struct window_state
{
    GLFWwindow* window;
};

bool window_create(window_state* state);
void window_destroy(window_state* state);

bool window_poolEvents(window_state* state);
void window_setContext(window_state* state);