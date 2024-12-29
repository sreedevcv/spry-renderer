#version 460 core

in vec3 TexCoord;
out vec4 fragColor;

uniform samplerCube cubeMap;

void main()
{
    fragColor = texture(cubeMap, TexCoord) * vec4(1.0, 0.9, 1.1, 1.0);
}