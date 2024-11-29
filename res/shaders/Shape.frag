#version 460 core

out vec4 fragColor;

in vec2 textureCoord;

uniform sampler2D baseTexture;

void main()
{
    fragColor = texture(baseTexture, textureCoord);
}