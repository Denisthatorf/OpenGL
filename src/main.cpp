#define GLFW_INCLUDE_VULKAN
#include <GL/glew.h>

#include "renderer.hpp"
#include "window.hpp"

int main()
{
    window_state state;
    window_create(&state);

    renderer_init(&state);

    while(window_poolEvents(&state))
    {
        render_test_draw();
    }

    window_destroy(&state);
}

//int main() {
//  // start GLEW extension handler
//  glewExperimental = GL_TRUE;
//  glewInit();
//
//  // get version info
//
//  // tell GL to only draw onto a pixel if the shape is closer to the viewer
//  glEnable(GL_DEPTH_TEST); // enable depth-testing
//  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
//
//  return 0;
//}