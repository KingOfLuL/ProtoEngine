#version 460 core

#include "ShaderValues.glsl"

in FRAGMENT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 VertexColor;
} _Fragment;

#include "Material.glsl"

uniform samplerCube _Skybox;

out vec4 _OutputColor;