include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG e2c92645460f680fd272fd2eed591efb2be7dc31
)

set(GLFW_USE_WAYLAND ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 33b0eb9fa336ffd8551024b1d2690e418014553b
)
set(GLM_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(glm)


add_library(
    renderer
    glad.c
    Window.cpp
    VAO.cpp
    Shader.cpp
)

target_link_libraries(
    renderer
    glfw
    glm::glm
)

target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
