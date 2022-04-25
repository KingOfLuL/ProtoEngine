#version 460 core

#include "Fragment.glsl"
#include "Material.glsl"
#include "ShaderValues.glsl"

in Fragment _Fragment;

uniform samplerCube _Skybox;

out vec4 _OutputColor;