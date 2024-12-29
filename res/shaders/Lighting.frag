#version 460 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform Material material;

void main()
{
    vec3 ambient = material.ambient * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diffStrength = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = (diffStrength * material.diffuse) * lightColor;

    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = (spec * material.specular) * lightColor;

    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, 1.0);
}