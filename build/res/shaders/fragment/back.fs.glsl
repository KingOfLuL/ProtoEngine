#include "Fragmentshader.glsl"

void main()
{
    vec3 col = texture(_ShadowMap, _Fragment.UV).rrr;
    // col = linearDepth(col);
    // vec3 col = texture2D(_DepthTexture, _Fragment.UV).rgb;
   
    _OutputColor = vec4(col, 1.0);
}