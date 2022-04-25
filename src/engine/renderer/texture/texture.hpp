#ifndef SRC_ENGINE_RENDERER_TEXTURE_TEXTURE
#define SRC_ENGINE_RENDERER_TEXTURE_TEXTURE

#include "glad/glad.h"
#include "renderer/buffers/buffers.hpp"
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
        Texture() = default;
        virtual ~Texture() = default;

        void virtual bind() const;
        void virtual setParameter(GLenum paramenter, GLenum value);

        uint32_t ID;
        GLenum colorFormat;
        int width;
        int height;
    };

    class Texture2D : public Texture
    {
    public:
        Texture2D() = default;
        Texture2D(uint32_t id, int w, int h, GLenum colorFormat, const TextureType &type, const std::string &path = "");

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

    public:
        void virtual setParameter(GLenum paramenter, GLenum value) override;
        void virtual bind() const override;

    private:
        std::array<std::string, 6> m_FacePaths;
    };

    void loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType typeName, std::vector<Texture2D> &textures, std::vector<Texture2D> &loadedTextures);
    Texture2D loadTextureFromFile(const std::string &path, TextureType type);
    void createGLTexture(uint32_t &id, int w, int h, GLenum colorFormat, const void *data, GLenum minFilter, GLenum maxFilter, GLenum wrapS, GLenum wrapT, bool mipmap);
}
#endif // SRC_ENGINE_RENDERER_TEXTURE_TEXTURE
