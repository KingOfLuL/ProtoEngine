#include "Lights.glsl"

// calculate directional light
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(_TBN * -light.direction);
    vec3 halfWayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);

    float spec = pow(max(dot(normal, halfWayDir), 0.0), _Material.Shininess);

    vec4 ambient = vec4(light.ambient, 1.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff;
    vec4 specular = vec4(light.specular, 1.0) * spec * _TextureSpecularStrength;

    return (ambient + (diffuse + specular) * (1.0 - shadow)) * _TextureDiffuseColor;
}
// calculate point light
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, float shadow) 
{
    vec3 tangentPosition = _TBN * light.position;

    vec3 lightDir = normalize(tangentPosition - _Fragment.TangentPosition);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfWayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfWayDir), 0.0), _Material.Shininess);

    float dst = length(light.position - _Fragment.Position);
    float attenuation = 1.0 / (light.constant + light.linear * dst + light.quadratic * dst);    

    vec4 ambient  = vec4(light.ambient, 1.0);
    vec4 diffuse  = vec4(light.diffuse, 1.0)  * diff;
    vec4 specular = vec4(light.specular, 1.0) * spec;

    ambient  *= vec4(vec3(attenuation), 1.0);
    diffuse  *= vec4(vec3(attenuation), 1.0);
    specular *= vec4(vec3(attenuation), 1.0);

    return (ambient + (diffuse + specular) * (1.0 - shadow)) * _TextureDiffuseColor;
}
// calculate spot light
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, float shadow)
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

    vec4 ambient = vec4(light.ambient, 1.0); 
    vec4 diffuse = vec4(light.diffuse, 1.0)   * diff; 
    vec4 specular = vec4(light.specular, 1.0) * spec; 

    ambient *= vec4(vec3(attenuation * intensity), 1.0); 
    diffuse *= vec4(vec3(attenuation * intensity), 1.0); 
    specular *= vec4(vec3(attenuation * intensity), 1.0); 
    
    return (ambient + (diffuse + specular) * (1.0 - shadow)) * _TextureDiffuseColor;
}
float calculateShadow(vec4 fragPos, float bias)
{
    vec3 projCoords = fragPos.xyz / fragPos.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if (projCoords.z > 1.0)
        return 0.0;

    float shadow = 0.0;
    float currentDepth = projCoords.z;
    vec2 texelSize = 1.0 / textureSize(_ShadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = - 1; y <= 1; ++y)
        {
            float pcfDepth = texture2D(_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

vec4 calculateLitColor()
{
    vec4 result = vec4(0.0);
    vec3 normal = texture2D(_Material.NormalMap, _Fragment.UV).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 viewDirection = normalize(_TBN * _ViewPosition - _Fragment.TangentPosition);

    float bias = max(_ShadowBiasMax * (1.0 - dot(_Fragment.Normal, _ShadowCasterLightDirection)), _ShadowBiasMin);
    float shadow = calculateShadow(_Fragment.LightSpacePosition, bias);

    for (int i = 0; i < MAX_NR_DIRLIGHTS; i++)
        if (_DirLights[i].isActive)
            result += CalcDirLight(_DirLights[i], normal, viewDirection, shadow);

    for (int i = 0; i < MAX_NR_POINTLIGHTS; i++)
        if (_PointLights[i].isActive)
            result += CalcPointLight(_PointLights[i], normal, viewDirection, shadow);

    for (int i = 0; i < MAX_NR_SPOTLIGHTS; i++)
        if (_SpotLights[i].isActive)
            result += CalcSpotLight(_SpotLights[i], normal, viewDirection, shadow);

    return result;
}