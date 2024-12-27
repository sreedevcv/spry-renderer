include(FetchContent)

# GLFW
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

# ASSIMP
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp
    GIT_TAG 01231d0e6001f555c81dcfcc6c581fa5797ccac9
)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)

# FreeType
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype
    GIT_TAG VER-2-13-3
)

FetchContent_MakeAvailable(freetype)

# SpdLog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0
)

FetchContent_MakeAvailable(spdlog)


# Imgui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.6
)

FetchContent_MakeAvailable(imgui)

# set(IMGUI_FILES
#     ${imgui_SOURCE_DIR}/imgui.cpp
#     # ${imgui_SOURCE_DIR}/imgui_draw.cpp
#     ${imgui_SOURCE_DIR}/imgui_internal.cpp
#     # ${imgui_SOURCE_DIR}/imgui_tables.cpp
#     # ${imgui_SOURCE_DIR}/imgui_.cpp
# )



add_library(
    renderer
    glad.c
    Window.cpp
    VAO.cpp
    Shader.cpp
    Camera.cpp
    Scene.cpp
    DefaultScene.cpp
    Texture.cpp
    Font.cpp
    FontRenderer.cpp
    IndexedVAO.cpp
    FrameBuffer.cpp
    RenderBuffer.cpp
    TextureRenderTarget.cpp
    shapes/Line.cpp
    shapes/Cuboid.cpp
    shapes/Plane.cpp
    shapes/Sphere.cpp
    utils/FontManager.cpp
    utils/ShaderManager.cpp
    utils/Timer.cpp
    utils/Toggle.cpp
    Mesh.cpp
    Model.cpp
    CubeMap.cpp
)

target_include_directories(renderer PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)  # added becaiuse #include <glad/glad.h> was not working
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/shapes)
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils)
target_include_directories(renderer PUBLIC ${imgui_SOURCE_DIR}/)

add_library(
    imgui_gui
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp # REMOVE this file; not needed
)

target_include_directories(imgui_gui PUBLIC ${imgui_SOURCE_DIR}/)

target_link_libraries(imgui_gui
    glfw
)

target_link_libraries(
    renderer
    glfw
    glm::glm
    assimp
    freetype
    spdlog
    imgui_gui
)
