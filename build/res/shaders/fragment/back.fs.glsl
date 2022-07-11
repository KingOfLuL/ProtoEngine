#include "Fragmentshader.glsl"

uniform sampler2D _RenderTexture;

void main()
{
    float col = texture(_RenderTexture, _Fragment.UV).r;
    col = linearDepth(col);
    // vec3 col = texture2D(_RenderTexture, _Fragment.UV).rgb;
   
    _OutputColor = vec4(vec3(col), 1.0);
}