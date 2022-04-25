#version 460 core

#include "Fragment.glsl"

layout (location = 0) in vec3 m_Position;
layout (location = 1) in vec3 m_Normal;
layout (location = 2) in vec2 m_UV;
layout (location = 3) in vec3 m_Color;

#include "ShaderValues.glsl"

out Fragment _Fragment;

uniform mat4 _ModelMatrix;

layout (std140, binding = 0) uniform Matrices
{
    mat4 _ViewMatrix;
    mat4 _ProjectionMatrix;
};

void setFragment()
{
    _Fragment.UV = m_UV;
    _Fragment.Normal = mat3(transpose(inverse(_ModelMatrix))) * m_Normal;
    _Fragment.Position = (_ModelMatrix * vec4(m_Position, 1.0)).xyz;
    _Fragment.VertexColor = m_Color;
}
vec4 calculatePosition()
{
    return _ProjectionMatrix * _ViewMatrix * _ModelMatrix * vec4(m_Position, 1.0);
}