#include "VertexBase.glsl"

void main()
{
    setFragment();
    gl_Position = calculatePosition();
}