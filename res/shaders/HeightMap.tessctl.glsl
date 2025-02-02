// tesselation control shader
#version 460 core

layout(vertices = 4) out; // GL_PATCH_VERTICES defined in code

in vec2 texCoord[];

out vec2 textureCoord[];

uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    textureCoord[gl_InvocationID] = texCoord[gl_InvocationID];

    // Invocation zero control tesselation for the entire patch
    if (gl_InvocationID == 0) {
        const int MIN_TESS_LEVEL = 4;
        const int MAX_TESS_LEVEL = 64;
        const float MIN_DISTANCE = 20;
        const float MAX_DISTANCE = 800;

        // Transform each vertex to view space
        vec4 viewSpaceVertex00 = view * model * gl_in[0].gl_Position;
        vec4 viewSpaceVertex01 = view * model * gl_in[1].gl_Position;
        vec4 viewSpaceVertex10 = view * model * gl_in[2].gl_Position;
        vec4 viewSpaceVertex11 = view * model * gl_in[3].gl_Position;

        // Distance from camera in [0, 1]
        float distance00 = clamp((abs(viewSpaceVertex00.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance01 = clamp((abs(viewSpaceVertex01.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance10 = clamp((abs(viewSpaceVertex10.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance11 = clamp((abs(viewSpaceVertex11.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        // Interpolate tesselation level based on closest vertex
        float tesselLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00));
        float tesselLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance00));
        float tesselLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11));
        float tesselLevel3 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance11));

        // Outer edge tesselation
        gl_TessLevelOuter[0] = tesselLevel0;
        gl_TessLevelOuter[1] = tesselLevel1;
        gl_TessLevelOuter[2] = tesselLevel2;
        gl_TessLevelOuter[3] = tesselLevel3;

        gl_TessLevelInner[0] = max(tesselLevel3, tesselLevel1);
        gl_TessLevelInner[1] = max(tesselLevel0, tesselLevel2);
    }
}
