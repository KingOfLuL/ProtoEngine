#ifndef SRC_ENGINE_RENDERER_MATERIAL_MATERIAL
#define SRC_ENGINE_RENDERER_MATERIAL_MATERIAL

#include <vector>
#include "shader/shader.hpp"
#include "renderer/texture/texture.hpp"

namespace Engine
{
    class Texture2D;
    enum TextureType : i32;

    struct Material
    {
    public:
        Material() = default;
        Material(const std::string &name);

    public:
        std::string name;
        Shader *shader = nullptr;
        std::vector<Texture2D *> textures;
        bool hasDiffuseTexture = true;
        bool hasSpecularTexture = true;
        bool hasNormalMap = true;
        glm::vec3 diffuseColor = glm::vec3(1.f);
        f32 shininess = 32.f;
        bool twoSided = false;

    public:
        static Material *getMaterialByName(const std::string &matName);
        static Texture2D *loadMaterialTexture(const std::string &path);

    private:
        static std::vector<Material *> s_LoadedMaterials;
    };
}
#endif // SRC_ENGINE_RENDERER_MATERIAL_MATERIAL
