#version 460 core

in float height;

out vec4 fragColor;

vec4 lerp(float val, vec4 a, vec4 b) {
    return (a * val) + ((1.0 - a) * b);
}

void main()
{
    vec4 waterLightColor = vec4(0.0, 0.35, 0.65, 1.0);
    vec4 waterDarkColor = vec4(0.0, 0.1, 0.3, 1.0);

    vec4 grassLightColor = vec4(0.35, 0.8, 0.25, 1.0);
    vec4 grassDarkColor = vec4(0.15, 0.45, 0.15, 1.0);

    float h = (height + 64.0) / 256.0;
    float seaLevel = 0.5;

    if (h > seaLevel) {
        float aboveSeaLevel = (h - seaLevel) / (1.0 - seaLevel);
        fragColor = lerp(aboveSeaLevel, waterLightColor, waterDarkColor);
    } else {
        float belowSeaLevel = h / seaLevel;
        fragColor = lerp(belowSeaLevel, grassLightColor, grassDarkColor);
    }
    // fragColor = vec4(h, h, h, 1.0);
}
