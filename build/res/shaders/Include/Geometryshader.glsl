#version 460 core

in VS_OUT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 VertexColor;
    vec3 TangentPosition;
    mat3 _TBN;
} _Vertices[];

out FRAGMENT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 VertexColor;
    vec3 TangentPosition;
    mat3 _TBN;
} _Vertex;