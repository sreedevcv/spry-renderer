#version 460 core

in vec3 fragPos;

// This is a float because we are writing to a cubemap texture (our shadowmap)
// whose internal format we have defined as FLOAT
out float fragColor;

uniform vec3 lightPosition;

void main()
{
    vec3 lightToVertex = lightPosition - fragPos;
    float dist = length(lightToVertex);
    fragColor = dist;
}