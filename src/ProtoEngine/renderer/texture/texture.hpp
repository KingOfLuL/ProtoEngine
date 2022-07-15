#ifndef SRC_PROTOENGINE_RENDERER_TEXTURE_TEXTURE
#define SRC_PROTOENGINE_RENDERER_TEXTURE_TEXTURE

#include "glad/glad.h"
#include "renderer/buffers/buffers.hpp"
#include "renderer/material/material.hpp"
#include <list>
#include <assimp/material.h>

namespace Engine
{
    enum TextureType : i32
    {
        MULTISAMPLE = 0x01,
        MAT_DIFFUSE = 0x02,
        MAT_SPECULAR = 0x04,
        MAT_NORMAL = 0x08,
        RENDER_TEXTURE = 0x10,
        DEPTH_TEXTURE = 0x20,
        COLOR_TEXTURE = 0x40,
    };

    class Texture
    {
    public:
        Texture() = default;
        virtual ~Texture() = default;

    public:
        void bind() const;
        void unbind() const;
        void setParameter(GLenum parameter, GLenum value);
        void setTextureWrapMode(GLenum wrapS, GLenum wrapT);
        void setTextureFilterMode(GLenum min, GLenum mag);

        u32 getID() const;

    public:
        i32 width;
        i32 height;
        GLenum colorFormat;
        // name used in the shader
        std::string name;

    protected:
        GLenum m_GLTextureType;
        u32 m_ID;
    };

    class Texture2D : public Texture
    {
    public:
        Texture2D() = default;
        Texture2D(i32 w,
                  i32 h,
                  i32 texType = TextureType::MAT_DIFFUSE,
                  bool mipmap = true,
                  void *data = nullptr,
                  GLenum colFormat = GL_RGB,
                  const std::string &path = "");

    public:
        std::string getPath() const;
        TextureType getType() const;

        static Texture2D loadFromFile(const std::string &path, TextureType type);

    public:
        static std::list<Texture2D> s_LoadedTextures;

    private:
        TextureType m_Type;
        std::string m_Path = "";
        bool m_Multisample;
    };

    class RenderTexture
    {
    public:
        RenderTexture() = default;
        RenderTexture(i32 w, i32 h, i32 texturesToRender);
        ~RenderTexture();

    public:
        void bindRender() const;
        void bindColorTexture() const;
        void unbindRender() const;

        Texture2D *getColorTexture();
        Texture2D *getTexture(i32 textureType);

        i32 width, height;

    private:
        bool m_AntiAliasing;
        Renderbuffer m_Renderbuffer;
        Texture2D m_ColorTexture;
        Texture2D m_DepthTexture;
        Texture2D m_MultisampleTexture;
        Framebuffer m_Framebuffer;
        Framebuffer m_MultisampleFramebuffer;
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
