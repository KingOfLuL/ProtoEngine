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
        glm::vec3 tangent;
        glm::vec3 bitangent;
        glm::vec3 color;

        Vertex();
        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, glm::vec3 color = glm::vec3(1.f));
    };
    struct Bounds
    {
        Bounds();

        glm::vec3 center = glm::vec3(0.f);
        glm::vec3 size = glm::vec3(0.f);

        void draw();

    private:
        Vertexbuffer m_Vertexbuffer;
    };

    struct Mesh
    {
        Mesh() = default;
        Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

        void calculateBounds();

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Bounds bounds;

        Vertexbuffer vertexbuffer;
    };
}
#endif // SRC_ENGINE_RENDERER_MESH_MESH
