#include "Fragmentshader.glsl"

uniform sampler2D screenTex;

void main()
{
    vec3 col = texture(screenTex, _Fragment.UV).rgb;
    _OutputColor = vec4(col , 1.0);
}