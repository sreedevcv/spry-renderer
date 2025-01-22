#version 460 core

in vec4 fragPos;

uniform vec3 lightPosition;
uniform float farPlane;

void main()
{
    vec3 lightToVertex = lightPosition - fragPos.xyz;
    float dist = length(lightToVertex) / farPlane;
    gl_FragDepth = dist;
}