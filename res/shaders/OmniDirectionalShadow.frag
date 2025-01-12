#version 460 core

in vec3 fragPos;

out float fragColor;

uniform vec3 lightPosition;

void main()
{
    vec3 lightToVertex = lightPosition - fragPos;
    float dist = length(lightToVertex);
    fragColor = dist;
}