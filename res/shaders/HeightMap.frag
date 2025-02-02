#version 460 core

in float height;

out vec4 fragColor;

void main()
{
    float h = (height + 16.0) / 64.0;
    fragColor = vec4(h, h, h, 1.0);
}
