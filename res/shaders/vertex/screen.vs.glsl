#include "Vertexshader.glsl"

void main()
{
    setFragment();
    _Fragment.Position = m_Position;
    gl_Position = vec4(_Fragment.Position.x, _Fragment.Position.y, 0.0, 1.0);
}