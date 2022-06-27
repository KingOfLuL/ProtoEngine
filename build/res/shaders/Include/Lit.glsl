#include "Lights.glsl"

vec4 calculateLitColor(vec3 normal, vec3 viewDirection)
{
    vec4 result = vec4(0.0);

    for (int i = 0; i < MAX_NR_DIRLIGHTS; i++)
        if (_DirLights[i].isActive)
            result += CalcDirLight(_DirLights[i], normal, viewDirection);

    for (int i = 0; i < MAX_NR_POINTLIGHTS; i++)
        if (_PointLights[i].isActive)
            result += CalcPointLight(_PointLights[i], normal, _Fragment.Position, viewDirection);

    for (int i = 0; i < MAX_NR_SPOTLIGHTS; i++)
        if (_SpotLights[i].isActive)
            result += CalcSpotLight(_SpotLights[i], normal, _Fragment.Position, viewDirection);

    return result;
}