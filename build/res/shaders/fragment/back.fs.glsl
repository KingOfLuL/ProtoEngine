#include "Fragmentshader.glsl"

void main()
{
    float col = texture(_ShadowMap, _Fragment.UV).r;
    // col = linearDepth(col);
    // vec3 col = texture2D(_DepthTexture, _Fragment.UV).rgb;
   
    _OutputColor = vec4(vec3(col), 1.0);
}