#version 460 core

out vec4 fragColor;

in VS_OUT {
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
    vec4 fragPosLightSpace;
} fs_in;

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
uniform sampler2DShadow shadowMap; 
// Options
uniform int useBlinnPhongModel;
uniform int useDirectionalLights;
uniform int usePointLights;
uniform int useSpotLights;
uniform int shadowSampling;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
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

    return ambient + ((1.0 - shadow) * (diffuse + specular));
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDir, float shadow)
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

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(fragPosition - light.position);
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

const 
vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

// Random number in the range [0, 1]
float randFloat(vec4 seed)
{
    float dotProduct = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dotProduct) * 43758.5453);
}

// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
// using sampler2DShadow
float shadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 norm)
{
    float bias = max(0.05 * (1.0 - dot(lightDir, norm)), 0.005);
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0, 1]
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 1.0;     // FIXME::should i return 1.0 or 0.0??
    
    projCoords.z -= bias;

    // Sample from sorrounding texels to get smoother shadows [percentage-closer filtering]
    // /*
        float shadow = 0.0;
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for (int x = -shadowSampling; x <= shadowSampling; x++) {
            for (int y = -shadowSampling; y <= shadowSampling; y++) {
                vec2 offset = vec2(x, y) * texelSize;
                vec3 samplePoint = vec3(projCoords.xy + offset, projCoords.z);
                float pcfDepth = texture(shadowMap, samplePoint);
                shadow += pcfDepth;
            }
        }
        shadow /= pow((2.0 * shadowSampling + 1.0), 2.0);
    // */

    // Poisson disk sampling
    /*
        float shadow = 0.0;
        for (int i = 0; i < shadowSampling; i++) {
            int index = int(16.0 * randFloat(vec4(fs_in.fragPos.xyz * 1000.0, i))) % 16;    // based on camera position
            // int index = int(16.0 * randFloat(vec4(gl_FragCoord.xyy, i))) % 16;           // based on fragment position
            vec2 offset = projCoords.xy + poissonDisk[index] / 700.0;
            float sampleValue = texture(shadowMap, vec3(offset.xy, projCoords.z));
            shadow += (1.0 / float(shadowSampling)) * (1.0 - sampleValue);
        }
    */


    return  1.0 - shadow;
}

void main()
{
    vec3 norm = normalize(fs_in.normal);
    vec3 viewDir = normalize(viewPosition - fs_in.fragPos);

    vec3 lightDir = normalize(-dirLight.direction);
    float shadow = shadowCalculation(fs_in.fragPosLightSpace, lightDir, norm);

    vec3 result = vec3(0.0);

    if (useDirectionalLights == 1) {
        result += calcDirLight(dirLight, norm, viewDir, shadow);
    }

    // if (usePointLights == 1) {
    //     for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
    //         result += calcPointLight(pointLights[i], norm, fs_in.fragPos, viewDir, shadow);
    //     }
    // }

    // if (useSpotLights == 1) {
    //     result += calcSpotLight(spotLight, norm, fs_in.fragPos, viewDir, shadow);
    // }

    fragColor = vec4(result, 1.0);
}