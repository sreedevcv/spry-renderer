# SpryRenderer: An OpenGL Renderer

SpryRenderer is an opengl renderer written in C++. It  contains RAII wrappers for OpenGL components along with 
- A Blinn Phong Renderer
- A perlin noise height map generator using Tesselation shaders
- A simple GUI shader editor using ImGUI 

Based on https://learnopengl.com/

## Build And Run
```bash
mkdir build && cd build
cmake ..
make
```

Cmake Script will automatically fetch dependencies like 
- glfw
- glm
- assimp
- freetype
- spdlog
- imgui
