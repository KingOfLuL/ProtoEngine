#include "Fragmentshader.glsl"

uniform sampler2D _RenderTexture;

void main()
{
    vec3 col = texture(_RenderTexture, _Fragment.UV).rgb;

    _OutputColor = vec4(col , 1.0);
    _OutputColor.rgb = pow(_OutputColor.rgb, vec3(1.0 / _Gamma));
}