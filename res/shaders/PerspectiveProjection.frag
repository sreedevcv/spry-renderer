#version 460 core

out vec4 fragColor;

in vec2 textureCoord;

uniform sampler2D shadowMap;
uniform float nearPlane;
uniform float farPlane;

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
    float depthValue = texture(shadowMap, textureCoord).r;
    fragColor = vec4(vec3(linearizeDepth(depthValue) / farPlane), 1.0);
}