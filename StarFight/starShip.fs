#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    //sampler2D specular;
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

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight[2];
uniform PointLight pointLight;
uniform int dirNum;
uniform int pointNum;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = {0,0,0};

    //direction light
    for(int i = 0; i < 2; i++){
        result += CalcDirLight(dirLight[i], norm, FragPos, viewDir);  
    }

    /*
    //point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // spot lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    }
    */
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a dir light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    float spec = 0.0;
    if (diff > 0.0)
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }  
    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    float spec = 0.0;
    if (diff > 0.0)
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }


    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)); specular map이 있다면
    vec3 specular = light.specular * (spec * material.specular);
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}