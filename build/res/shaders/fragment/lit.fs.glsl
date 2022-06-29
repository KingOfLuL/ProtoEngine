#include "Fragmentshader.glsl"
#include "Lit.glsl"

void main() 
{
    getTextureColors();

    vec3 normal = normalize(_Fragment.Normal);
    vec3 viewDirection = normalize(_ViewPosition - _Fragment.Position);

    _OutputColor = calculateLitColor(normal, viewDirection);
}