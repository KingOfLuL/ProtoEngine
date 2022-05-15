#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in FRAGMENT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 VertexColor;
} _Fragments[];

out FRAGMENT
{
    vec3 Position;
    vec2 UV;
    vec3 Normal;
    vec3 VertexColor;
} _Fragment;

void main() 
{    
    for (int i = 0; i < gl_in.length(); i++)
    {
        _Fragment.Position = _Fragments[i].Position;
        _Fragment.UV = _Fragments[i].UV;
        _Fragment.Normal = _Fragments[i].Normal;
        _Fragment.VertexColor = _Fragments[i].VertexColor;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
} 