#include "Fragmentshader.glsl"

void main() 
{
    _OutputColor = texture(_Skybox, _Fragment.Position);
}