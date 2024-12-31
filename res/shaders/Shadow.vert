#version 460 core

layout (location = 0) in vec3 Position;

uniform mat4 lightViewProj;
uniform mat4 model;

void main()
{
    gl_Position = lightViewProj * model * vec4(Position, 1.0);
}