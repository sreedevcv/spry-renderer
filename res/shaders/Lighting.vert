#version 460 core

layout (location = 0) in vec3 positionIn;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 textureCoord;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(positionIn, 1.0);
    normal = mat3(transpose(inverse(model))) * normalIn;
    fragPos = vec3(model * vec4(positionIn, 1.0));
    texCoord = textureCoord;
}