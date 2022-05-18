#include "Vertexshader.glsl"

void main() 
{
    setVertex();
    _Vertex.Position = m_Position;
    vec4 pos = _ProjectionMatrix * mat4(mat3(_ViewMatrix)) * vec4(_Vertex.Position, 1.0);
    gl_Position = pos.xyww;
}