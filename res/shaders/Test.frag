#version 460 core

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;

void main()
{
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 norm = normalize(normal);
    float diff = max(dot(lightDir, norm), 0.0);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, norm), 0.0), material.shininess * 4.0);

    vec3 ambient = dirLight.ambient * material.ambient;
    vec3 diffuse = dirLight.diffuse * diff * material.diffuse;
    vec3 specular = dirLight.specular * spec * material.specular;

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}