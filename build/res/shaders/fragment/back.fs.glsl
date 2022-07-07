#include "Fragmentshader.glsl"

uniform sampler2D _RenderTexture;

void main()
{
    vec3 col = texture(_RenderTexture, _Fragment.UV).rgb;

    if (_Fragment.UV.x >= 0.99 || _Fragment.UV.y >= 0.99 ||
        _Fragment.UV.x <= 0.01 || _Fragment.UV.y <= 0.01)
        _OutputColor = vec4(vec3(0.0), 1.0);
    else
        _OutputColor = vec4(col , 1.0);
}