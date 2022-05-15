#include "FragmentBase.glsl"
#include "Reflection.glsl"

void main()
{
    getTextureColors();
    
    vec4 reflection = calcReflection(_Fragment);
    vec4 refraction = calcRefraction(_Fragment, 1.33);

    _OutputColor = vec4(reflection.rgb, 1.0); 
}