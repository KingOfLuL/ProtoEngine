#ifndef SRC_ENGINE_VERTICES
#define SRC_ENGINE_VERTICES

#include "renderer/mesh/mesh.hpp"

namespace Engine
{
    const Vertex QUAD_VERTICES[4] = {
        Vertex({-1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}),
        Vertex({-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}),
        Vertex({1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}),
        Vertex({1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}),
    };
    const uint32_t QUAD_INDICES[6] = {0, 1, 3, 1, 2, 3};

    const Vertex CUBEMAP_VERTICES[36] = {
        Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),

        Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),

        Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),

        Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),

        Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),

        Vertex({-1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
        Vertex({1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f})

    };
}
#endif // SRC_ENGINE_VERTICES
