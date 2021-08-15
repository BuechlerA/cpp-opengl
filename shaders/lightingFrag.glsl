#version 330 core
out vec4 outColor;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;
    float shininess;
};

uniform sampler2D flashlightTexture;

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight{
    vec3 position;

    // point light attenuation
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NUM_POINT_LIGHTS 4
uniform PointLight pointLights[NUM_POINT_LIGHTS];

struct SpotLight{
    float cutOff;
    float outerCutOff;
};
uniform SpotLight spotLight;

in vec3 FragPos;
in vec3 Normal;
in vec2 Texcoord;

uniform sampler2D texFace;
uniform sampler2D texContainer;

uniform vec3 viewPosition;
uniform Material material;

uniform vec2 viewPort;
uniform float time;

// void main()
// {
//     //vec3 lightDirection = normalize(light.position - FragPos); // point light
//     vec3 lightDirection = normalize(-light.direction); // directional light

//     //ambient
//     //vec3 ambient = light.ambient * texture(material.diffuseMap, Texcoord).rgb;
//     light.ambient = calcAmbientLight();
//     //diffuse
//     vec3 norm = normalize(Normal);
    
//     float diff = max(dot(norm, lightDirection), 0.0f);
//     vec3 diffuse = light.diffuse * diff * texture(material.diffuseMap, Texcoord).rgb;
//     //specular
//     vec3 viewDirection = normalize(viewPosition - FragPos);
//     vec3 reflectDirection = reflect(-lightDirection, norm);
//     float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
//     vec3 specular = light.specular * spec * texture(material.specularMap, Texcoord).rgb;

//     //spotlight 
//     float theta = dot(lightDirection, normalize(-light.direction)); // spot light inner circle angle
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
//     vec2 fragCoords = gl_FragCoord.xy / viewPort * vec2(1.0f, -1.0f);
//     intensity *= length(vec3(texture(flashlightTexture, fragCoords)));
//     diffuse *= intensity;
//     specular *= intensity;

//     // attenuation
//     float distance = length(light.position - FragPos);
    
//     float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//     attenuation *= length(vec3(texture(flashlightTexture, fragCoords))); //apply spotlight cookie texture
    
//     diffuse *= attenuation;
//     specular *= attenuation;
//     vec3 result = ambient + diffuse + specular;
//     outColor = vec4(result, 1.0f);
// }

// vec3 calcAmbientLight()
// {
//     vec3 ambient = light.ambient * texture(material.diffuseMap, Texcoord).rgb;
//     return ambient;
// }

vec3 calcDirectionalLight(DirLight dirLight, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-dirLight.direction);

    float diff = max(dot(normal, lightDirection), 0.0f);

    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);

    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuseMap, Texcoord));
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuseMap, Texcoord));
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specularMap, Texcoord));
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - FragPos);
    //diffuse
    float diff = max(dot(normal, lightDirection), 0.0f);
    // specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance +
                                light.quadratic * (distance * distance));
    // combine
    vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, Texcoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, Texcoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, Texcoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return(ambient + diffuse + specular);
}

// vec3 calcSpotLight()
// {

// }

// vec3 calcEmmisionMap()
// {
//     //emission
//     //vec3 emission = vec3(0.0f);
//     // if (texture(material.specularMap, Texcoord).r == 0.0f)
//     // {
//     //     emission = texture(material.emissionMap, Texcoord).rgb;
//     //     emission = texture(material.emissionMap, Texcoord + vec2(0.0f, time/1000.0f)).rgb;
//     //     emission = emission * (sin(time/1000.0f) * 0.5 + 0.5) * 2.0;
//     // }
//     // vec3 result = ambient + diffuse + specular + emission;
// }

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragPos);

    vec3 result = vec3(0.0f);

    result += calcDirectionalLight(dirLight, norm, viewDirection);
    for(int i = 0; i < NUM_POINT_LIGHTS; i++)
        result += calcPointLight(pointLights[i], norm, FragPos, viewDirection);

    //spotlights

    outColor = vec4(result, 1.0f);
}