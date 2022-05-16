#include "VertexShader.glsl"

void main() 
{
    setFragment();
    _Fragment.Position = m_Position;
    vec4 pos = _ProjectionMatrix * mat4(mat3(_ViewMatrix)) * vec4(_Fragment.Position, 1.0);
    gl_Position = pos.xyww;
}