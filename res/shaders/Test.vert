#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 0) in vec2 inTexCoord;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(inPosition, 1.0);
    normal = mat3(transpose(inverse((model)))) * inNormal;
    texCoord = inTexCoord;
    fragPos = vec3(model * vec4(inPosition, 1.0));
}