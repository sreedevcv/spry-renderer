#version 460

out vec4 fragColor;

in vec3 vertColor;

void main()
{
    // fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    fragColor = vec4(vertColor.xyz, 1.0);
}