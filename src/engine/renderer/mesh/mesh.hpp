#ifndef SRC_ENGINE_RENDERER_MESH_MESH
#define SRC_ENGINE_RENDERER_MESH_MESH

#include "renderer/buffers/buffers.hpp"
#include "Assimp/mesh.h"

namespace Engine
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 color;

        Vertex();
        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, glm::vec3 color);
    };

    struct Mesh
    {
        Mesh() = default;
        Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        Vertexbuffer vertexbuffer;
    };
}
#endif // SRC_ENGINE_RENDERER_MESH_MESH
