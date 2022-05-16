#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

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
        _Fragment.Position = _Vertices[i].Position;
        _Fragment.UV = _Vertices[i].UV;
        _Fragment.Normal = _Vertices[i].Normal;
        _Fragment.VertexColor = _Vertices[i].VertexColor;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
} 