#include "libs.hpp"

#include "mesh.hpp"

#include "glad/glad.h"

namespace Engine
{
    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) : vertices(vertices), indices(indices)
    {
        vertexbuffer = Vertexbuffer(&vertices[0], vertices.size());
        vertexbuffer.addIndexbuffer(&indices[0], indices.size());
    }

    Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, glm::vec3 color)
        : position(position), normal(normal), uv(uv), color(color)
    {
    }
    Vertex::Vertex()
        : position(glm::vec3(0.0f)), normal(glm::vec3(0.0f)), uv(glm::vec2(0.0f)), color(glm::vec3(1.0f))
    {
    }
}