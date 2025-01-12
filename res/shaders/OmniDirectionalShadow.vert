#version 460 core

layout (location = 0) in vec3 Position;

uniform mat4 viewProj;
uniform mat4 model;

out vec3 fragPos;

void main()
{
    vec4 pos = model * vec4(Position, 1.0);
    fragPos = pos.xyz;
    gl_Position = viewProj * pos;
}