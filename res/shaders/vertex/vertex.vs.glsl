#include "Vertexshader.glsl"

void main()
{
    setVertex();
    gl_Position = calculatePosition();
}