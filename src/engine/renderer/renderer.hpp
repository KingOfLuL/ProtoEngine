#ifndef SRC_ENGINE_RENDERER_RENDERER
#define SRC_ENGINE_RENDERER_RENDERER

#define MAX_NR_POINTLIGHTS 5
#define MAX_NR_SPOTLIGHTS 5
#define MAX_NR_DIRLIGHTS 2

#include "material/material.hpp"
#include "texture/texture.hpp"
#include "entity/components.hpp"

namespace Engine::Renderer
{
    extern Shader *shaderLit;
    extern Shader *shaderBounds;

    void init();
    void updateLights();
    void render();
}

#endif // SRC_ENGINE_RENDERER_RENDERER
