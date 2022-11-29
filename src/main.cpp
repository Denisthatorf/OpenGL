#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
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
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "ui.hpp"

struct Point
{
    glm::vec3 pos;
    glm::vec3 color;
};

std::vector<Point> Deserialize(const std::string& path) {
    std::vector<Point> result;
    std::ifstream file;
    file.open(path);

    if (!file.is_open())
    {
        //TODO: error
        perror("Error open");
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(getline(file, line)) {
        int i = 0;
        Point point;

        //TODO:
        point.color = glm::vec3(60, 60, 60);

        std::istringstream ss(line);
        std::string token;

        //while(ss >> str)
        while (getline(ss, token, ' '))
        {    
            i += 1;

            switch (i) {
                case 1:
                    point.pos.x = std::stof(token);
                    continue;
                case 2:
                    point.pos.y = std::stof(token);
                    continue;
                case 3:
                    point.pos.z = std::stof(token);
                    continue;
            }

            //TODO: error
            perror("Bad format in file");
            exit(EXIT_FAILURE);
        }

        if(i != 3)
        {
            //TODO: error
            perror("Bad format in file");
            exit(EXIT_FAILURE);
        }
        
        result.push_back(point);
 
    }

    file.close();
    return result;
}

//TODO: remove
//std::vector<Point> Deserialize2(const std::string& path)
//{
//    std::vector<Point> result;
//
//    FILE* file = fopen(path.c_str(), "r");
//
//    if (file == NULL) {
//      perror("Error open");
//      exit(EXIT_FAILURE);
//    }
//    
//    size_t read;
//    size_t length = 0;
//    char* line = NULL;
//    char* delim = " ";
//
//    while ((read = getline(&line, &length, file)) != -1) {
//        Point point;
//        int i = 0;
//
//        char* token = strtok(line, delim);
//        while (token)
//        {
//            i += 1;
//
//            printf("%s\n", token);
//            token = strtok(NULL, delim);
//
//            switch (i) {
//                case 1:
//                    point.pos.x = std::stof(token);
//                    continue;
//                case 2:
//                    point.pos.y = std::stof(token);
//                    continue;
//                case 3:
//                    point.pos.z = std::stof(token);
//                    continue;
//            }
//
//            //TODO: error
//            perror("Bad format in file");
//            exit(EXIT_FAILURE);
//        }
//
//        if(i != 3)
//        {
//            //TODO: error
//            perror("Bad format in file");
//            exit(EXIT_FAILURE);
//        }
//        
//        result.push_back(point);
//    }
//
//    fclose(file);
//
//    return result;
//}

int main() {
    Window window(900, 800, "Hello World");
    window.SetContext();
    window.InitImGui();
    
    if (glewInit() != GLEW_OK)
        std::cout << "glewInit error!" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        std::vector<Point> points = Deserialize("/home/denis/Projects/C++/OpenGL/out/ascii.txt");

        GL_ASSERT (glEnable(GL_BLEND));
        GL_ASSERT (glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        //VertexArray va;
        //VertexBuffer vb(points.data(), points.size()*sizeof(*points.data()));

        //VertexBufferLayout layout;
        //layout.Push<float>(3);
        //layout.Push<float>(3);
        //va.AddBuffer(vb, layout);
        
        glm::mat4 proj = glm::ortho(0.0f, 900.0f, 0.0f, 800.0f, -1.0f, 1.0f);
        glm::mat4 view(1.0f); view = glm::translate(view, glm::vec3(-100, 0, 0));
        glm::vec3 translation = glm::vec3(200, 200, 0);

        Shader shader("/home/denis/Projects/C++/OpenGL/shaders/point.shader");
        shader.Bind();

        GLuint vbo_;
        GLuint vao_;

        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(*points.data()), points.data(), GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), 0);
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)(sizeof(glm::vec3)));
        glEnableVertexAttribArray(1);

        //va.Unbind();
        //vb.Unbind();
        shader.Unbind();

        Renderer renderer;

        // Loop until the user closes the window
        while (window.PoolEvents())
        {
            GL_ASSERT(glClear(GL_COLOR_BUFFER_BIT));

            shader.Bind();

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            //va.Bind();
            //vb.Bind();

            glDrawArrays(GL_POINTS, 0, points.size());

            UI_draw();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
        }
    }

    glfwTerminate();
    return 0;
}