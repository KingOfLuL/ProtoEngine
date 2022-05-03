#include "libs.hpp"

#include "engine.hpp"
#include "entity/entity.hpp"
#include "mesh.hpp"
#include "renderer/renderer.hpp"
#include "util/util.hpp"
#include "glad/glad.h"

namespace Engine
{
    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) : vertices(vertices), indices(indices)
    {
        vertexbuffer = Vertexbuffer(&vertices[0], vertices.size());
        vertexbuffer.addIndexbuffer(&indices[0], indices.size());

        calculateBounds();
    }
    void Mesh::calculateBounds()
    {
        auto [minX, maxX] = std::minmax_element(vertices.begin(), vertices.end(),
                                                [](const Vertex &a, const Vertex &b)
                                                {
                                                    return a.position.x < b.position.x;
                                                });
        auto [minY, maxY] = std::minmax_element(vertices.begin(), vertices.end(),
                                                [](const Vertex &a, const Vertex &b)
                                                {
                                                    return a.position.y < b.position.y;
                                                });
        auto [minZ, maxZ] = std::minmax_element(vertices.begin(), vertices.end(),
                                                [](const Vertex &a, const Vertex &b)
                                                {
                                                    return a.position.z < b.position.z;
                                                });
        /* cube corner vertices */
        glm::vec3 a((*minX).position.x, (*maxY).position.y, (*minZ).position.z);
        glm::vec3 b((*minX).position.x, (*minY).position.y, (*minZ).position.z);
        glm::vec3 c((*maxX).position.x, (*minY).position.y, (*minZ).position.z);
        glm::vec3 d((*maxX).position.x, (*maxY).position.y, (*minZ).position.z);
        glm::vec3 e((*minX).position.x, (*maxY).position.y, (*maxZ).position.z);
        glm::vec3 f((*minX).position.x, (*minY).position.y, (*maxZ).position.z);
        glm::vec3 g((*maxX).position.x, (*minY).position.y, (*maxZ).position.z);
        glm::vec3 h((*maxX).position.x, (*maxY).position.y, (*maxZ).position.z);

        bounds.center = g + (a - g) * 0.5f;
        bounds.size = glm::vec3((d - a).x, (b - a).y, (e - a).z);

        auto data = GeomUtil::createLineCubeVertices(a, b, c, d, e, f, g, h);
        m_boundsVertexbuffer = Vertexbuffer(&data[0], 36);
    }
    void Mesh::drawBounds() const
    {
        Renderer::shaderColor.use();
        Renderer::shaderColor.setMat4("_ModelMatrix", glm::mat4(1.0));
        m_boundsVertexbuffer.bind();
        glDrawArrays(GL_LINES, 0, 48);
        m_boundsVertexbuffer.unbind();
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