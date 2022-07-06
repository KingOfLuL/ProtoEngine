#include "libs.hpp"

#include "skybox.hpp"

#include "glad/glad.h"
#include "renderer/mesh/mesh.hpp"
#include "renderer/renderer.hpp"

namespace Engine
{
    Skybox::Skybox(const std::array<std::string, 6> &faces)
        : m_Cubemap(faces)
    {
        const Vertex verts[] =
            {
                Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  // a
                Vertex({-1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}), // b
                Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  // c
                Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  // c
                Vertex({1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),   // d
                Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  // a

                Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  // f
                Vertex({-1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}), // b
                Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  // a
                Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  // a
                Vertex({-1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),   // e
                Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  // f

                Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}), //
                Vertex({1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),   //
                Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),   //
                Vertex({1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}), //

                Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}), //
                Vertex({-1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),   //
                Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),   //
                Vertex({1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}), //

                Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}), //
                Vertex({1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),   //
                Vertex({1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),   //
                Vertex({-1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({-1.0f, 1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}), //

                Vertex({-1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}), //
                Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({1.0f, -1.0f, -1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({-1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),  //
                Vertex({1.0f, -1.0f, 1.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}),
            };
        m_Vertexbuffer = Vertexbuffer(&verts[0], 36);
    }

    void Skybox::draw() const
    {
        glDepthFunc(GL_LEQUAL);
        glActiveTexture(GL_TEXTURE0);

        Shader::getShaderByName("Cubemap")->use();
        m_Cubemap.bind();
        m_Vertexbuffer.draw();

        glDepthFunc(GL_LESS);
    }
}
