#version 460 core

layout (location = 0) in vec3 m_Position;
layout (location = 1) in vec3 m_Normal;
layout (location = 2) in vec2 m_UV;
layout (location = 3) in vec3 m_Color;

#include "Values.glsl"
#include "Matrices.glsl"

out FRAGMENT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 VertexColor;
} _Vertex;

void setVertex()
{
    _Vertex.UV = m_UV;
    _Vertex.Normal = mat3(transpose(inverse(_ModelMatrix))) * m_Normal;
    _Vertex.Position = (_ModelMatrix * vec4(m_Position, 1.0)).xyz;
    _Vertex.VertexColor = m_Color;
}
vec4 calculatePosition()
{
    return _ProjectionMatrix * _ViewMatrix * _ModelMatrix * vec4(m_Position, 1.0);
}