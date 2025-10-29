// Tesselation evaluation shader
#version 460 core

layout(quads, fractional_odd_spacing, ccw) in;

uniform sampler2D heightMap;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

in vec2 textureCoord[];

out float height;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // Texture Coordinates
    vec2 t00 = textureCoord[0];
    vec2 t01 = textureCoord[1];
    vec2 t10 = textureCoord[2];
    vec2 t11 = textureCoord[3];

    // Bilinear interpolation
    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 texCoord = (t1 - t0) * v + t0;

    // Texel lookup
    height = texture(heightMap, texCoord).y * 256.0 - 64.0;

    // Postion Coordinates
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // Normal Computation
    vec4 uDir = p01 - p00;
    vec4 vDir = p10 - p00;
    vec4 normal = normalize(vec4(cross(uDir.xyz, vDir.xyz), 1.0));

    // Bilinear interpolation
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    // Displacing height along normal
    p += normal * height;

    // Output patch position in clip space
    gl_Position = proj * view * model * p;
}
