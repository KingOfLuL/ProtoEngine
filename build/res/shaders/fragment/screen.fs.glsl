#include "Fragmentshader.glsl"

uniform sampler2D _RenderTexture;

void main()
{
    vec3 col = texture2D(_RenderTexture, _Fragment.UV).rgb;
    _OutputColor = vec4(col , 1.0);

    // float depth = texture2D(_DepthTexture, _Fragment.UV).r;
    // depth = linearDepth(depth);
    // _OutputColor = vec4(vec3(depth), 1.0);

    _OutputColor.rgb = correctGamma(_OutputColor.rgb);
}