#version 460 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform float ambientStrength;

void main()
{
    // float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diffStrength = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diffStrength * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32.0);
    vec3 specular = spec * specularStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    fragColor = vec4(result, 1.0);
}