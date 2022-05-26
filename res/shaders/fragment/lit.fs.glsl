#include "Fragmentshader.glsl"
#include "Lights.glsl"

void main() 
{
    getTextureColors();

    vec3 norm = normalize(_Fragment.Normal);
    vec3 viewDir = normalize(_ViewPosition - _Fragment.Position);

    vec4 result = vec4(0.0);

    for (int i = 0; i < _NumDirLights; i++)
        result += CalcDirLight(_DirLights[i], norm, viewDir);

    for (int i = 0; i < _NumPointLights; i++)
        result += CalcPointLight(_PointLights[i], norm, _Fragment.Position, viewDir);

    for (int i = 0; i < _NumSpotLights; i++)
        result += CalcSpotLight(_SpotLights[i], norm, _Fragment.Position, viewDir);
        
    _OutputColor = result;
    // _OutputColor = vec4(1.0, 0.0, 1.0, 1.0);
}