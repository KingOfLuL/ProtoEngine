#include "Vertexshader.glsl"

void main() 
{
    setVertex();
    gl_Position = vec4(_Vertex.Position.x, _Vertex.Position.y, 0.0, 1.0);
}