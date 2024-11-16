#version 460 core

in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D text;
uniform vec4 color;

void main()
{
    float alpha = texture(text, texCoords).r;
    vec4 sampled = vec4(1.0, 1.0, 1.0, alpha);
    fragColor = vec4(color.xyz, 1.0) * sampled;
}