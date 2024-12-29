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

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutOffAngle;
    float outerCutOffAngle;
};

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform Material material;
uniform SpotLight spotLight;

float spotLightIntensity()
{
    // from light to fragment
    vec3 lightDir = normalize(fragPos - spotLight.position);
    float viewAngle = acos(dot(normalize(spotLight.direction), lightDir));
    float interpolated = (viewAngle - spotLight.outerCutOffAngle) / (spotLight.innerCutOffAngle - spotLight.outerCutOffAngle);
    float intensity = clamp(interpolated, 0.0, 1.0);

    return intensity;
}

// int isInsideSpotLight()
// {
//     vec3 lightDir = normalize(spotLight.position - fragPos);
//     float theta = dot(lightDir, normalize(-spotLight.direction));


//     if (theta > spotLight.cutOffAngle) {
//         return 1;
//     } else {
//         return 0;
//     }
// }


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

    vec3 result = ambient + ((diffuse + specular) * spotLightIntensity());
    


    fragColor = vec4(result, 1.0);
}