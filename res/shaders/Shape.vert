#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 textureCoord;

// out VS_OUT {
//     vec2 texCoords;
// } vs_out;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
    textureCoord = texCoord;
    // vs_out.texCoords = texCoord;
}