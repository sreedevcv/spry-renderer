#version 460 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D tex1;

void main()
{
    fragColor = texture(tex1, texCoords);
}