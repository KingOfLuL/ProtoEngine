#include "skybox.hpp"

#include "util/vertices.hpp"
#include "glad/glad.h"

namespace Engine
{
    Skybox::Skybox(const std::array<std::string, 6> &faces)
        : m_Cubemap(faces),
          m_Shader("vertex/cubemap.vs.glsl", "fragment/cubemap.fs.glsl", "Cubemap"),
          m_Vertexbuffer(&CUBEMAP_VERTICES, 36)
    {
    }
    void Skybox::draw() const
    {
        glDepthFunc(GL_LEQUAL);

        m_Shader.use();
        m_Cubemap.bind();
        m_Vertexbuffer.draw();

        glDepthFunc(GL_LESS);
    }
}
