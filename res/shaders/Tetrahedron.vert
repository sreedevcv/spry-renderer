#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 vertColor;

void main()
{
    vec4 pos = proj * view * model * vec4(position, 1.0);
    vertColor = vertexColor;
    gl_Position = pos;
}