#include "Geometryshader.glsl"
#include "Matrices.glsl"

layout(points) in;
layout(line_strip, max_vertices = 18) out;

#define matrices _ProjectionMatrix * _ViewMatrix * _ModelMatrix

const float epsilon = 0.001;

void main()
{
    vec3 size = _Vertices[0].Normal + vec3(epsilon); 
    vec4 center = gl_in[0].gl_Position;

    // front
    gl_Position = (center + (matrices * vec4(-size.x, size.y, size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-size.x, -size.y, size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(size.x, -size.y, size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(size.x, size.y, size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-size.x, size.y, size.z, 1.0)));
    EmitVertex();
    EndPrimitive();

    // back
    gl_Position = (center + (matrices * vec4(-size.x, size.y, -size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-size.x, -size.y, -size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(size.x, -size.y, -size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(size.x, size.y, -size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-size.x, size.y, -size.z, 1.0)));
    EmitVertex();
    EndPrimitive();

    // left
    gl_Position = (center + (matrices * vec4(-size.x, size.y, size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-size.x, size.y, -size.z, 1.0)));
    EmitVertex();
    EndPrimitive();
    gl_Position = (center + (matrices * vec4(-size.x, -size.y, size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-size.x, -size.y, -size.z, 1.0)));
    EmitVertex();
    EndPrimitive();

    // right
    gl_Position = (center + (matrices * vec4(size.x, size.y, size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(size.x, size.y, -size.z, 1.0)));
    EmitVertex();
    EndPrimitive();
    gl_Position = (center + (matrices * vec4(size.x, -size.y, size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(size.x, -size.y, -size.z, 1.0)));
    EmitVertex();
    EndPrimitive();
}