// tesselation control shader
#version 460 core

layout(vertices = 4) out; // GL_PATCH_VERTICES defined in code

in vec2 texCoord[];

out vec2 textureCoord[];

void main()
{
     gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
     textureCoord[gl_InvocationID] = texCoord[gl_InvocationID];

     // Invocation zero control tesselation for the entire patch
     if (gl_InvocationID == 0) {
         gl_TessLevelOuter[0] = 16;
         gl_TessLevelOuter[1] = 16;
         gl_TessLevelOuter[2] = 16;
         gl_TessLevelOuter[3] = 16;
         
         gl_TessLevelInner[0] = 16;
         gl_TessLevelInner[1] = 16;
     }
}
