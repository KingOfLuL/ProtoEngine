#include "VertexShader.glsl"

void main()
{
    setFragment();

    gl_Position = _ProjectionMatrix * vec4(_Fragment.Position.xy, 0.0, 1.0);
}