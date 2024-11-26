#version 460 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D texture_diffuse;

void main()
{
    fragColor = texture(texture_diffuse, texCoords);
}
