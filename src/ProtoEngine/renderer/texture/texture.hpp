#ifndef SRC_ENGINE_RENDERER_TEXTURE_TEXTURE
#define SRC_ENGINE_RENDERER_TEXTURE_TEXTURE

#include "glad/glad.h"
#include "renderer/buffers/buffers.hpp"
#include "renderer/material/material.hpp"
#include <list>
#include <assimp/material.h>

namespace Engine
{
    enum class TextureType
    {
        DIFFUSE,
        SPECULAR,
        NORMAL,
        RENDERTEXTURE,
    };

    class Texture
    {
    public:
        Texture() = default;
        virtual ~Texture() = default;

    public:
        void bind() const;
        void setParameter(GLenum parameter, GLenum value);
        void setTextureWrapMode(GLenum wrapS, GLenum wrapT);
        void setTextureFilterMode(GLenum min, GLenum mag);

        uint32_t getID() const;

    public:
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
        Texture2D(void *data, int w, int h, GLenum colFormat, const TextureType &texType = TextureType::DIFFUSE, const std::string &path = "");

    public:
        std::string getPath() const;
        TextureType getType() const;

        static Texture2D loadFromFile(const std::string &path, const TextureType &type);

    public:
        static std::list<Texture2D> s_LoadedTextures;

    private:
        TextureType m_Type;
        std::string m_Path = "";
    };

    class RenderTexture
    {
    public:
        RenderTexture() = default;
        RenderTexture(int w, int h);
        ~RenderTexture();

    public:
        void bindFramebuffer() const;
        void unbindFramebuffer() const;
        void bindTexture() const;

        int getWidth() const;
        int getHeight() const;

        Texture2D *getTexture() const;

    private:
        Framebuffer m_Framebuffer;
        Texture2D *m_Texture = nullptr;
    };

    class Cubemap : public Texture
    {
    public:
        Cubemap() = default;
        Cubemap(const std::array<std::string, 6> &faces);

    private:
        std::array<std::string, 6> m_FacePaths;
    };
}
#endif // SRC_ENGINE_RENDERER_TEXTURE_TEXTURE
