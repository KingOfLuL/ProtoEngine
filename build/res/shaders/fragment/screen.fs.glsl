#include "Fragmentshader.glsl"

void main()
{
    vec3 col = texture2D(_RenderTexture, _Fragment.UV).rgb;
    _OutputColor = vec4(col , 1.0);

    _OutputColor.rgb = correctGamma(_OutputColor.rgb);
}