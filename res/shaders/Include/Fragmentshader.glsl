#version 460 core

#include "Values.glsl"

in FRAGMENT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 VertexColor;
    vec3 TangentPosition;
} _Fragment;

in mat3 _TBN;

#include "Material.glsl"

uniform samplerCube _Skybox;

out vec4 _OutputColor;

vec3 correctGamma(vec3 color)
{
    return pow(color, vec3(1.0 / _Gamma));
}