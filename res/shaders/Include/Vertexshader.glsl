#version 460 core

layout (location = 0) in vec3 m_Position;
layout (location = 1) in vec3 m_Normal;
layout (location = 2) in vec2 m_UV;
layout (location = 3) in vec3 m_Tangent;
layout (location = 4) in vec3 m_Bitangent;
layout (location = 5) in vec3 m_Color;

#include "Values.glsl"
#include "Matrices.glsl"

out FRAGMENT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 VertexColor;
    vec3 TangentPosition;
    vec4 LightSpacePosition;
} _Vertex;

out mat3 _TBN;

void setVertex()
{
    _Vertex.Position = (_ModelMatrix * vec4(m_Position, 1.0)).xyz;
    _Vertex.Normal = m_Normal;
    _Vertex.UV = m_UV;
    _Vertex.Tangent = m_Tangent;
    _Vertex.Bitangent = m_Bitangent;
    _Vertex.VertexColor = m_Color;

    mat3 normalMatrix = transpose(inverse(mat3(_ModelMatrix)));
    vec3 T = normalize(normalMatrix * _Vertex.Tangent);
    vec3 N = normalize(normalMatrix * _Vertex.Normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    _TBN = transpose(mat3(T, B, N));
    _Vertex.TangentPosition = _TBN * _Vertex.Position;
    _Vertex.LightSpacePosition = _LightSpaceMatrix * vec4(_Vertex.Position, 1.0);
}
vec4 calculatePosition()
{
    return _ProjectionMatrix * _ViewMatrix * _ModelMatrix * vec4(m_Position, 1.0);
}