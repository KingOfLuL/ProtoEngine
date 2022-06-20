#include "Fragmentshader.glsl"
#include "Lights.glsl"

void main() 
{
    getTextureColors();

    vec3 norm = normalize(_Fragment.Normal);
    vec3 viewDir = normalize(_ViewPosition - _Fragment.Position);

    vec4 result = vec4(0.0);

    for (int i = 0; i < MAX_NR_DIRLIGHTS; i++)
        if (_DirLights[i].isActive)
            result += CalcDirLight(_DirLights[i], norm, viewDir);

    for (int i = 0; i < MAX_NR_POINTLIGHTS; i++)
        if (_PointLights[i].isActive)
            result += CalcPointLight(_PointLights[i], norm, _Fragment.Position, viewDir);

    for (int i = 0; i < MAX_NR_SPOTLIGHTS; i++)
        if (_SpotLights[i].isActive)
            result += CalcSpotLight(_SpotLights[i], norm, _Fragment.Position, viewDir);
        
    _OutputColor = result;
}