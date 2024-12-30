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

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutOffAngle;
    float outerCutOffAngle;

    float constant;
    float linear;
    float quadratic;
};

#define POINT_LIGHT_COUNT 4

uniform vec3 viewPosition;
uniform Material material;
uniform SpotLight spotLight;
uniform DirLight dirLight;
uniform PointLight pointLights[POINT_LIGHT_COUNT];
// Options
uniform int useBlinnPhongModel;
uniform int useDirectionalLights;
uniform int usePointLights;
uniform int useSpotLights;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    float spec = 0.0;
    if (useBlinnPhongModel == 1) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 4.0);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    }
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * material.diffuse * diff;
    vec3 specular = light.specular * material.specular * spec;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPosition);
    // Diffuse Shading
    float diff = max(dot(lightDir, normal), 0.0);
    // Specular Shading
    float spec = 0.0;
    if (useBlinnPhongModel == 1) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 4.0);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    }
    // Attenuation
    float dist = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * material.diffuse * diff;
    vec3 specular = light.specular * material.specular * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDir)
{
    vec3 lightDir = normalize(fragPos - light.position);
    float viewAngle = acos(dot(normalize(light.direction), lightDir));
    float interpolated = (viewAngle - light.outerCutOffAngle) / (light.innerCutOffAngle - light.outerCutOffAngle);
    float intensity = clamp(interpolated, 0.0, 1.0);
    // Attenuation
    float dist = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 ambient = attenuation * intensity * material.ambient;
    vec3 diffuse = attenuation * intensity * material.diffuse;
    vec3 specular = attenuation * intensity * material.specular;

    return ambient + diffuse + specular;
}

// float spotLightIntensity()
// {
//     // from light to fragment
//     vec3 lightDir = normalize(fragPos - spotLight.position);
//     float viewAngle = acos(dot(normalize(spotLight.direction), lightDir));
//     float interpolated = (viewAngle - spotLight.outerCutOffAngle) / (spotLight.innerCutOffAngle - spotLight.outerCutOffAngle);
//     float intensity = clamp(interpolated, 0.0, 1.0);

//     return intensity;
// }

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPosition - fragPos);

    // vec3 result = calcDirLight(dirLight, norm, viewDir);
    vec3 result = vec3(0.0);

    if (useDirectionalLights == 1) {
        result += calcDirLight(dirLight, norm, viewDir);
    }

    if (usePointLights == 1) {
        for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
            result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
        }
    }

    if (useSpotLights == 1) {
        result += calcSpotLight(spotLight, norm, fragPos, viewDir);
    }

    fragColor = vec4(result, 1.0);
}