#version 460 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D finalTexture;

void main()
{
    vec4 color = texture(finalTexture, uv);
//     color.r = 0.0f;
    fragColor = color;
}
