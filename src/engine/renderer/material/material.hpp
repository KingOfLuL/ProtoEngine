#ifndef SRC_ENGINE_RENDERER_MATERIAL_MATERIAL
#define SRC_ENGINE_RENDERER_MATERIAL_MATERIAL

#include <vector>
#include "shader/shader.hpp"
#include "renderer/texture/texture.hpp"

namespace Engine
{
    class Texture2D;

    struct Material
    {
    public:
        Material() = default;
        Material(const std::string &name);

    public:
        std::string name;
        Shader *shader;
        std::vector<Texture2D> textures;
        bool hasDiffuseTexture;
        bool hasSpecularTexture;
        glm::vec3 diffuseColor = glm::vec3(1.f);
        float shininess = 32.f;
        bool twoSided = false;

    public:
        static Material *getMaterialByName(const std::string &matName);

    private:
        static std::vector<Material *> s_LoadedMaterials;
    };
}
#endif // SRC_ENGINE_RENDERER_MATERIAL_MATERIAL
