#include "libs.hpp"

#include "Texture.hpp"

#include "glad/glad.h"
#include "util/util.hpp"

#include <stb/stb_image.h>

namespace Engine
{
    std::list<Texture2D> Texture2D::s_LoadedTextures;

    void Texture::setParameter(GLenum parameter, GLenum value)
    {
        glTexParameteri(m_GLTextureType, parameter, value);
    }
    void Texture::bind() const
    {
        glBindTexture(m_GLTextureType, m_ID);
    }
    void Texture::unbind() const
    {
        glBindTexture(m_GLTextureType, m_ID);
    }
    void Texture::setTextureWrapMode(GLenum wrapS, GLenum wrapT)
    {
        setParameter(GL_TEXTURE_WRAP_S, wrapS);
        setParameter(GL_TEXTURE_WRAP_T, wrapT);
    }
    void Texture::setTextureFilterMode(GLenum min, GLenum mag)
    {
        setParameter(GL_TEXTURE_MIN_FILTER, min);
        setParameter(GL_TEXTURE_MAG_FILTER, mag);
    }
    u32 Texture::getID() const
    {
        return m_ID;
    }

    Texture2D Texture2D::loadFromFile(const std::string &path, const TextureType &type)
    {
        std::string fileName = PathUtil::FULL_PATH + PathUtil::TEXTURE_PATH + path;

        i32 width, height, nrComponents;
        void *data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);

        if (data == nullptr)
        {
            std::cout << "Texture failed to load at path: " << fileName << std::endl;
            return Texture2D();
        }

        GLenum format = GL_RGBA;
        switch (nrComponents)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        }
        return Texture2D(width, height, type, true, data, format, path);
    }
    Texture2D::Texture2D(i32 w, i32 h, i32 texType, bool mipmap, void *textureData, GLenum colFormat, const std::string &path)
        : m_Type((TextureType)texType), m_Path(path)
    {
        m_Multisample = m_Type & TextureType::MULTISAMPLE;

        if (m_Multisample)
            m_GLTextureType = GL_TEXTURE_2D_MULTISAMPLE;
        else
            m_GLTextureType = GL_TEXTURE_2D;

        width = w;
        height = h;
        colorFormat = colFormat;

        GLenum internalFormat = colorFormat;
        GLenum glType = GL_UNSIGNED_BYTE;

        if (m_Type == TextureType::MAT_DIFFUSE)
        {
            if (colorFormat == GL_RGB)
                internalFormat = GL_SRGB;
            else if (colorFormat == GL_RGBA)
                internalFormat = GL_SRGB_ALPHA;
        }
        else if (m_Type == TextureType::DEPTH_TEXTURE)
        {
            colorFormat = GL_DEPTH_COMPONENT;
            internalFormat = GL_DEPTH_COMPONENT;
            glType = GL_FLOAT;
        }

        glGenTextures(1, &m_ID);
        glBindTexture(m_GLTextureType, m_ID);

        if (m_Multisample)
            glTexImage2DMultisample(m_GLTextureType, 4, colorFormat, width, height, GL_TRUE);
        else
            glTexImage2D(m_GLTextureType, 0, internalFormat, width, height, 0, colorFormat, glType, textureData);

        if (mipmap)
            glGenerateMipmap(m_GLTextureType);

        setTextureWrapMode(GL_REPEAT, GL_REPEAT);
        if (mipmap)
            setTextureFilterMode(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        else if (m_Multisample)
            setTextureFilterMode(GL_LINEAR, GL_LINEAR);
        else if (m_Type == TextureType::DEPTH_TEXTURE)
            setTextureFilterMode(GL_NEAREST, GL_NEAREST);
        else
            setTextureFilterMode(GL_NEAREST, GL_LINEAR);

        unbind();
    }
    std::string Texture2D::getPath() const
    {
        return m_Path;
    }
    TextureType Texture2D::getType() const
    {
        return m_Type;
    }

    Cubemap::Cubemap(const std::array<std::string, 6> &faces) : m_FacePaths(faces)
    {
        m_GLTextureType = GL_TEXTURE_CUBE_MAP;

        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

        i32 w, h, nrChannels;
        colorFormat = GL_RGB;
        GLint sRGBformat = GL_SRGB;

        for (size_t i = 0; i < faces.size(); i++)
        {
            auto path = PathUtil::FULL_PATH + PathUtil::TEXTURE_PATH + faces[i];
            void *data = stbi_load(path.c_str(), &w, &h, &nrChannels, 0);
            if (data)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, sRGBformat, w, h, 0, colorFormat, GL_UNSIGNED_BYTE, data);
            else
                std::cout << "failed to load Cubemap at: " << faces[i] << std::endl;
            stbi_image_free(data);
        }

        setTextureFilterMode(GL_LINEAR, GL_LINEAR);
        setTextureWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    RenderTexture::RenderTexture(i32 w, i32 h, i32 texturesToRender)
        : width(w), height(h)
    {
        m_AntiAliasing = texturesToRender & TextureType::MULTISAMPLE;
        bool drawColorTex = texturesToRender & TextureType::COLOR_TEXTURE;
        bool drawDepthTex = texturesToRender & TextureType::DEPTH_TEXTURE;

        m_Framebuffer = Framebuffer(width, height);

        if (drawColorTex)
        {
            m_ColorTexture = Texture2D(width, height, TextureType::RENDER_TEXTURE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                   m_ColorTexture.getID(), 0);
        }
        else
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        if (drawDepthTex)
        {
            m_DepthTexture = Texture2D(width, height, TextureType::DEPTH_TEXTURE, false);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                                   m_DepthTexture.getID(), 0);
        }

        Framebuffer::checkError();

        if (m_AntiAliasing)
        {
            m_MultisampleTexture = Texture2D(width, height, TextureType::RENDER_TEXTURE | TextureType::MULTISAMPLE, false);
            m_MultisampleFramebuffer = Framebuffer(width, height);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
                                   m_MultisampleTexture.getID(), 0);
            Framebuffer::checkError();
        }
        if (m_AntiAliasing || (drawColorTex && !drawDepthTex))
            m_Renderbuffer = Renderbuffer(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_AntiAliasing);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    RenderTexture::~RenderTexture()
    {
    }
    void RenderTexture::bindColorTexture() const
    {
        m_ColorTexture.bind();
    }
    void RenderTexture::bindRender() const
    {
        if (m_AntiAliasing)
            m_MultisampleFramebuffer.bind();
        else
            m_Framebuffer.bind();
    }
    void RenderTexture::unbindRender() const
    {
        if (m_AntiAliasing)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MultisampleFramebuffer.getID());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer.getID());
            glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

            m_MultisampleFramebuffer.unbind();
        }
        else
        {
            m_Framebuffer.unbind();
        }
    }
    Texture2D *RenderTexture::getColorTexture()
    {
        return &m_ColorTexture;
    }
    Texture2D *RenderTexture::getTexture(i32 textureType)
    {
        if (textureType == TextureType::COLOR_TEXTURE)
            if (textureType & TextureType::MULTISAMPLE)
                return &m_MultisampleTexture;
            else
                return &m_ColorTexture;
        else if (textureType == TextureType::DEPTH_TEXTURE)
            return &m_DepthTexture;
        return nullptr;
    }
}