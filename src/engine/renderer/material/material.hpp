#ifndef SRC_ENGINE_RENDERER_MATERIAL_MATERIAL
#define SRC_ENGINE_RENDERER_MATERIAL_MATERIAL

#include "Shader/Shader.hpp"
#include "Renderer/Texture/Texture.hpp"

namespace Engine
{
    struct Material
    {
        Material() = default;
        Material(Shader *shader);

        Shader *shader;

        bool twoSided = false;

        std::vector<Texture2D> textures;
        bool hasDiffuseTexture;
        bool hasSpecularTexture;

        glm::vec3 diffuseColor = glm::vec3(1.f);

        float shininess = 32.f;
    };
}
#endif // SRC_ENGINE_RENDERER_MATERIAL_MATERIAL
