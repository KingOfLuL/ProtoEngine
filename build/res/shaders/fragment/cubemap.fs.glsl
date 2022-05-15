#include "FragmentBase.glsl"

void main() 
{
    _OutputColor = texture(_Skybox, _Fragment.Position);
}