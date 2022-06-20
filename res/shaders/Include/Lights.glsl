struct DirLight // 64 bytes -> 64 bytes (16 floats) => 0 bytes padding
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    bool isActive;
};
struct PointLight // 76 bytes -> 80 bytes (20 floats) => 4 bytes padding (1 floats)
{
    vec3 position; 
    
    vec3 ambient;  
    vec3 diffuse;   
    vec3 specular; 

    float constant;
    float linear;   
    float quadratic;

    bool isActive;
};
struct SpotLight // 100 bytes -> 112 bytes (28 floats) => 12 bytes padding (3 floats)
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    bool isActive;
};

#define MAX_NR_DIRLIGHTS 2
#define MAX_NR_POINTLIGHTS 5
#define MAX_NR_SPOTLIGHTS 5

layout (std140, binding = 1) uniform Lights
{
    DirLight _DirLights[MAX_NR_DIRLIGHTS];
    PointLight _PointLights[MAX_NR_POINTLIGHTS];
    SpotLight _SpotLights[MAX_NR_SPOTLIGHTS];
};


// calculate directional light
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) 
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), _Material.shininess);

    vec4 ambient = vec4(light.ambient, 1.0)   *         _TextureDiffuseColor;
    vec4 diffuse = vec4(light.diffuse, 1.0)   * diff *  _TextureDiffuseColor;
    vec4 specular = vec4(light.specular, 1.0) * spec *  _TextureSpecularColor;
    return (ambient + diffuse + specular);
}
// calculate point light
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) 
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), _Material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec4 ambient  = vec4(light.ambient, 1.0)  *        _TextureDiffuseColor;
    vec4 diffuse  = vec4(light.diffuse, 1.0)  * diff * _TextureDiffuseColor;
    vec4 specular = vec4(light.specular, 1.0) * spec * _TextureSpecularColor;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
// calculate spot light
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), _Material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec4 ambient = vec4(light.ambient, 1.0)   *         _TextureDiffuseColor; 
    vec4 diffuse = vec4(light.diffuse, 1.0)   * diff *  _TextureDiffuseColor; 
    vec4 specular = vec4(light.specular, 1.0) * spec *  _TextureSpecularColor; 

    ambient *= attenuation * intensity; 
    diffuse *= attenuation * intensity; 
    specular *= attenuation * intensity; 
    return (ambient + diffuse + specular);
}