#ifndef SRC_ENGINE_RENDERER_SKYBOX_SKYBOX
#define SRC_ENGINE_RENDERER_SKYBOX_SKYBOX

#include "renderer/texture/texture.hpp"
#include "renderer/material/shader/shader.hpp"
#include "renderer/buffers/buffers.hpp"

namespace Engine
{
    class Skybox
    {
    public:
        Skybox() = default;
        /// @param faces must be defined in following order: right, left, top, bottom, front, back
        Skybox(const std::array<std::string, 6> &faces);

    public:
        void draw() const;
        void bind() const;

    private:
        Cubemap m_Cubemap;
        Vertexbuffer m_Vertexbuffer;
    };
}

#endif // SRC_ENGINE_RENDERER_SKYBOX_SKYBOX
