#version 460 core

layout (location = 0) in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 TexCoord;

void main()
{
    vec4 pos = proj * view * model * vec4(position, 1.0);
    gl_Position = pos.xyww;
    TexCoord = position;
}