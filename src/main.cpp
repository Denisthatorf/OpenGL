#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "opengl/renderer.hpp"
#include "opengl/opengl_types.hpp"

#include "opengl/opengl_types.hpp"
#include "opengl/vertexBuffer.hpp"
#include "opengl/vertexBufferLayout.hpp"
#include "opengl/indexBuffer.hpp"
#include "opengl/vertexArray.hpp"
#include "opengl/shader.hpp"
#include "opengl/texture.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "window.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

int main(void)
{
    Window window(900, 800, "Hello World");
    window.SetContext();
    window.InitImGui();

    //TODO: remove
    glfwSwapInterval(1);
    
    if (glewInit() != GLEW_OK)
        std::cout << "glewInit error!" << std::endl;

    // Log the OpenGL version used because we can
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] =
        {
            100.0f, 100.0f, 0.0f, 0.0f,   //0
            200.0f, 100.0f, 1.0f, 0.0f,   //1
            200.0f,  200.0f, 1.0f, 1.0f,   //2
            100.0f,  200.0f,  0.0f, 1.0f   //3
        };
        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        GL_ASSERT (glEnable(GL_BLEND));
        GL_ASSERT (glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        
        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, 900.0f, 0.0f, 800.0f, -1.0f, 1.0f);

        glm::mat4 view(1.0f); 
        view = glm::translate(view, glm::vec3(-100, 0, 0));
        glm::vec3 translation = glm::vec3(200, 200, 0);

        Shader shader("/home/denis/Projects/C++/OpenGL/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        //TODO: make relative
        Texture texture("/home/denis/Projects/C++/OpenGL/texture/ava.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

                // Animation stuff
        float r = 0.0f;
        float increment = 0.05f;

        // Loop until the user closes the window
        while (window.PoolEvents())
        {
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //{
            //    static float f = 0.0f;
            //    static int counter = 0;

            //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            //    ImGui::Checkbox("Another Window", &show_another_window);

            //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //        counter++;
            //    ImGui::SameLine();
            //    ImGui::Text("counter = %d", counter);

            //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            //    ImGui::End();
            //}

            {
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 900.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);

            // Animate the r value between 0.0 and 1.0
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }

    glfwTerminate();
    return 0;
}