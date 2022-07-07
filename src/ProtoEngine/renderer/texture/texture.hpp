#ifndef SRC_PROTOENGINE_RENDERER_TEXTURE_TEXTURE
#define SRC_PROTOENGINE_RENDERER_TEXTURE_TEXTURE

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
        RENDER_TEXTURE,
        DEPTH_TEXTURE,
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

        u32 getID() const;

    public:
        i32 width;
        i32 height;
        GLenum colorFormat;

    protected:
        GLenum m_GLTextureType;
        u32 m_ID;
    };

    class Texture2D : public Texture
    {
    public:
        Texture2D() = default;
        Texture2D(void *data, i32 w, i32 h, GLenum colFormat, const TextureType &texType = TextureType::DIFFUSE, const std::string &path = "");

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

    class RenderTexture : public Texture2D
    {
    public:
        RenderTexture() = default;
        RenderTexture(i32 w, i32 h);
        ~RenderTexture();

    public:
        void bindFramebuffer() const;
        void unbindFramebuffer() const;
        // void bindTexture() const;

        // i32 getWidth() const;
        // i32 getHeight() const;

        // Texture2D *getTexture() const;

    private:
        Framebuffer m_Framebuffer;
        // Texture2D *m_Texture = nullptr;
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
#endif // SRC_PROTOENGINE_RENDERER_TEXTURE_TEXTURE
