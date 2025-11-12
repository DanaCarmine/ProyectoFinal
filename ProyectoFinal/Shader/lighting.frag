#version 330 core
#define NUMBER_OF_POINT_LIGHTS 8

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform Material material;

// Para el cubo de agua sin textura
uniform bool useTexture;
uniform vec3 objectColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Luz direccional
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // Luces puntuales
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    
    // Si no usa textura, aplicar transparencia
    if (!useTexture) {
        color = vec4(result, 0.5); // 0.5 = 50% transparente
    } else {
        color = vec4(result, 1.0);
    }
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Usar textura o color sólido
    vec3 baseColor = useTexture ? vec3(texture(material.diffuse, TexCoords)) : objectColor;
    vec3 specColor = useTexture ? vec3(texture(material.specular, TexCoords)) : vec3(1.0);
    
    vec3 ambient = light.ambient * baseColor;
    vec3 diffuse = light.diffuse * diff * baseColor;
    vec3 specular = light.specular * spec * specColor;
    
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // Usar textura o color sólido
    vec3 baseColor = useTexture ? vec3(texture(material.diffuse, TexCoords)) : objectColor;
    vec3 specColor = useTexture ? vec3(texture(material.specular, TexCoords)) : vec3(1.0);
    
    vec3 ambient = light.ambient * baseColor;
    vec3 diffuse = light.diffuse * diff * baseColor;
    vec3 specular = light.specular * spec * specColor;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}