#include "Lights.glsl"

// calculate directional light
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(_TBN * -light.direction);
    vec3 halfWayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);

    float spec = pow(max(dot(normal, halfWayDir), 0.0), _Material.Shininess);

    vec4 ambient = vec4(light.ambient, 1.0)   *         _TextureDiffuseColor;
    vec4 diffuse = vec4(light.diffuse, 1.0)   * diff *  _TextureDiffuseColor;
    vec4 specular = vec4(light.specular, 1.0) * spec *  _TextureSpecularColor;
    return (ambient + diffuse + specular);
}
// calculate point light
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) 
{
    vec3 tangentPosition = _TBN * light.position;

    vec3 lightDir = normalize(tangentPosition - _Fragment.TangentPosition);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfWayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfWayDir), 0.0), _Material.Shininess);

    float dst = length(light.position - _Fragment.Position);
    float attenuation = 1.0 / (light.constant + light.linear * dst + light.quadratic * dst);    

    vec4 ambient  = vec4(light.ambient, 1.0)  *        _TextureDiffuseColor;
    vec4 diffuse  = vec4(light.diffuse, 1.0)  * diff * _TextureDiffuseColor;
    vec4 specular = vec4(light.specular, 1.0) * spec * _TextureSpecularColor;

    ambient  *= vec4(vec3(attenuation), 1.0);
    diffuse  *= vec4(vec3(attenuation), 1.0);
    specular *= vec4(vec3(attenuation), 1.0);

    return (ambient + diffuse + specular);
}
// calculate spot light
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 tangentPosition = _TBN * light.position;

    vec3 lightDir = normalize(tangentPosition - _Fragment.TangentPosition);
    vec3 halfWayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfWayDir), 0.0), _Material.Shininess);

    float dst = length(tangentPosition - _Fragment.TangentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * dst + light.quadratic * dst);    

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec4 ambient = vec4(light.ambient, 1.0)   *         _TextureDiffuseColor; 
    vec4 diffuse = vec4(light.diffuse, 1.0)   * diff *  _TextureDiffuseColor; 
    vec4 specular = vec4(light.specular, 1.0) * spec *  _TextureSpecularColor; 

    ambient *= vec4(vec3(attenuation * intensity), 1.0); 
    diffuse *= vec4(vec3(attenuation * intensity), 1.0); 
    specular *= vec4(vec3(attenuation * intensity), 1.0); 
    
    return (ambient + diffuse + specular);
}

vec4 calculateLitColor()
{
    vec4 result = vec4(0.0);
    vec3 normal = texture2D(_Material.NormalMap, _Fragment.UV).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 viewDirection = normalize(_TBN * _ViewPosition - _Fragment.TangentPosition);

    for (int i = 0; i < MAX_NR_DIRLIGHTS; i++)
        if (_DirLights[i].isActive)
            result += CalcDirLight(_DirLights[i], normal, viewDirection);

    for (int i = 0; i < MAX_NR_POINTLIGHTS; i++)
        if (_PointLights[i].isActive)
            result += CalcPointLight(_PointLights[i], normal, viewDirection);

    for (int i = 0; i < MAX_NR_SPOTLIGHTS; i++)
        if (_SpotLights[i].isActive)
            result += CalcSpotLight(_SpotLights[i], normal, viewDirection);

    return result;
}