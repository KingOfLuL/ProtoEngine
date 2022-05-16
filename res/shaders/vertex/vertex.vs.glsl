#include "Vertexshader.glsl"

void main()
{
    setFragment();
    gl_Position = calculatePosition();
}