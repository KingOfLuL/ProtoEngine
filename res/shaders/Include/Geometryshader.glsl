#version 460 core

in FRAGMENT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 VertexColor;
} _Vertices[];

out FRAGMENT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 VertexColor;
} _Fragment;