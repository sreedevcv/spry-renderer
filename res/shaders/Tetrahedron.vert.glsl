#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 pos = projection * view * model * vec4(position, 0.0);
    gl_Position = pos;
}