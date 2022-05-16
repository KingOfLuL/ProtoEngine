#include "VertexShader.glsl"

void main()
{
    setFragment();
    gl_Position = calculatePosition();
}