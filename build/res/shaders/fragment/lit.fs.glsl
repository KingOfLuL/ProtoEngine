#include "Fragmentshader.glsl"
#include "Lit.glsl"

void main() 
{
    getTextureColors();

    _OutputColor = calculateLitColor();
}