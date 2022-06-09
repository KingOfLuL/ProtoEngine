#ifndef SRC_ENGINE_RENDERER_TEXTURE_TEXTURE
#define SRC_ENGINE_RENDERER_TEXTURE_TEXTURE

#include "glad/glad.h"
#include "renderer/buffers/buffers.hpp"
#include "renderer/material/material.hpp"
#include <vector>
#include <assimp/material.h>

namespace Engine
{
    enum class TextureType
    {
        DIFFUSE,
        SPECULAR,
        NORMAL,
    };

    struct Texture
    {
    public:
        Texture() = default;
        virtual ~Texture();

    public:
        void bind() const;
        void setParameter(GLenum parameter, GLenum value);
        void setTextureWrapMode(GLenum wrapS, GLenum wrapT);
        void setTextureFilterMode(GLenum min, GLenum mag);

        uint32_t getID() const;

    public:
        void *data = nullptr;
        int width;
        int height;
        GLenum colorFormat;

    protected:
        GLenum m_GLTextureType;
        uint32_t m_ID;
    };

    class Texture2D : public Texture
    {
    public:
        Texture2D() = default;
        Texture2D(void *data, int w, int h, GLenum colFormat, const TextureType &texType = TextureType::DIFFUSE, const std::string &path = "NULL");

        static Texture2D loadFromFile(const std::string &path, const TextureType &type);

    public:
        std::string getPath() const;
        TextureType getType() const;

    private:
        TextureType m_Type;
        std::string m_Path;
    };

    class RenderTexture : public Texture
    {
    public:
        RenderTexture() = default;
        RenderTexture(int w, int h);

    public:
        void bindFramebuffer() const;
        void unbindFramebuffer() const;
        void bindTexture() const;

    private:
        Framebuffer m_Framebuffer;
    };

    class Cubemap : public Texture
    {
    public:
        Cubemap() = default;
        Cubemap(const std::array<std::string, 6> &faces);

    private:
        std::array<std::string, 6> m_FacePaths;
    };

    extern std::vector<Texture2D> loadedTextures;

    class Material;

    void loadMaterialTexture(const std::string &path, TextureType texType, Material *material);
}
#endif // SRC_ENGINE_RENDERER_TEXTURE_TEXTURE
