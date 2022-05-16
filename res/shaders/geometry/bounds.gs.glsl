#include "Geometryshader.glsl"
#include "Matrices.glsl"

layout(points) in;
layout(line_strip, max_vertices = 24) out;

#define center (gl_in[0].gl_Position)

#define matrices _ProjectionMatrix * _ViewMatrix * _ModelMatrix

uniform vec3 size;

const float epsilon = 0.001;

void main()
{
    vec3 _size = size + vec3(epsilon); 

    // front
    gl_Position = (center + (matrices * vec4(-_size.x, _size.y, _size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-_size.x, -_size.y, _size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(_size.x, -_size.y, _size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(_size.x, _size.y, _size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-_size.x, _size.y, _size.z, 1.0)));
    EmitVertex();
    EndPrimitive();

    // back
    gl_Position = (center + (matrices * vec4(-_size.x, _size.y, -_size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-_size.x, -_size.y, -_size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(_size.x, -_size.y, -_size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(_size.x, _size.y, -_size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-_size.x, _size.y, -_size.z, 1.0)));
    EmitVertex();
    EndPrimitive();

    // left
    gl_Position = (center + (matrices * vec4(-_size.x, _size.y, _size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-_size.x, _size.y, -_size.z, 1.0)));
    EmitVertex();
    EndPrimitive();
    gl_Position = (center + (matrices * vec4(-_size.x, -_size.y, _size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(-_size.x, -_size.y, -_size.z, 1.0)));
    EmitVertex();
    EndPrimitive();

    // right
    gl_Position = (center + (matrices * vec4(_size.x, _size.y, _size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(_size.x, _size.y, -_size.z, 1.0)));
    EmitVertex();
    EndPrimitive();
    gl_Position = (center + (matrices * vec4(_size.x, -_size.y, _size.z, 1.0)));
    EmitVertex();
    gl_Position = (center + (matrices * vec4(_size.x, -_size.y, -_size.z, 1.0)));
    EmitVertex();
    EndPrimitive();
}