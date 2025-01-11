#version 460 core

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 textureCoord;

out VS_OUT {
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
    vec4 fragPosLightSpace;
    vec4 pointLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightViewProj;
uniform mat4 pointLightMat;

void main()
{
    vs_out.fragPos = vec3(model * vec4(positionIn, 1.0));
    vs_out.normal = transpose(inverse(mat3(model))) * normalIn;
    vs_out.texCoord = textureCoord;
    vs_out.fragPosLightSpace = lightViewProj * vec4(vs_out.fragPos, 1.0);
    vs_out.pointLightSpace = pointLightMat * vec4(vs_out.fragPos, 1.0);
    gl_Position = proj * view * vec4(vs_out.fragPos, 1.0);
}