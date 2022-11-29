cmake_minimum_required(VERSION 3.2)
project(Engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_RELEASE")

file(GLOB SOURCES_C "src/*.cpp" "src/*/*.cpp" "src/*/*/*.cpp" "src/*/*/*/*.cpp")
file(GLOB SOURCES_CXX "src/*.cpp" "src/*/*.cpp" "src/*/*/*.cpp" "src/*/*/*/*.cpp")

file(GLOB HEADERS_C "src/*.h" "src/*/*.h" "src/*/*/*.h" "src/*/*/*/*.h")
file(GLOB HEADERS_CXX "src/*.hpp" "src/*/*.hpp" "src/*/*/*.hpp" "src/*/*/*/*.hpp")

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
)

add_executable(Engine 
    ${SOURCES_C} ${SOURCES_CXX}
)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(Engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/
)

target_link_libraries(Engine
    PUBLIC
        glm
    PRIVATE
        glfw
        OpenGL::GLX
        GLEW::GLEW
        GLUT::GLUT
)