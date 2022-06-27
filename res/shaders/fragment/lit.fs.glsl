#include "Fragmentshader.glsl"
#include "Lit.glsl"

void main() 
{
    getTextureColors();

    vec3 norm = normalize(_Fragment.Normal);
    vec3 viewDir = normalize(_ViewPosition - _Fragment.Position);

    _OutputColor = calculateLitColor(norm, viewDir);
}