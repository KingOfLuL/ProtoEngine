#include "Fragmentshader.glsl"

uniform sampler2D _RenderTexture;

void main()
{
    float depth = texture(_RenderTexture, _Fragment.UV).r;
    depth = linearDepth(depth);

   
    _OutputColor = vec4(depth, depth, depth, 1.0);
}