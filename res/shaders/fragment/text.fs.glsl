#include "FragmentBase.glsl"

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, _Fragment.UV).r);
    _OutputColor = vec4(textColor, 1.0) * sampled;
}