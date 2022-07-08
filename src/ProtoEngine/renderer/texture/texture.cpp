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

    Texture2D::Texture2D(void *textureData, i32 w, i32 h, GLenum colFormat, const TextureType &texType, const std::string &path)
        : m_Type(texType), m_Path(path)
    {
        m_GLTextureType = GL_TEXTURE_2D;

        width = w;
        height = h;
        colorFormat = colFormat;

        GLenum sRGBformat = colorFormat;
        GLenum glType = GL_UNSIGNED_BYTE;
        if (m_Type == TextureType::DIFFUSE)
        {
            if (colorFormat == GL_RGB)
                sRGBformat = GL_SRGB;
            else if (colorFormat == GL_RGBA)
                sRGBformat = GL_SRGB_ALPHA;
        }
        else if (m_Type == TextureType::DEPTH_TEXTURE)
        {
            colorFormat = GL_DEPTH_COMPONENT;
            sRGBformat = GL_DEPTH_COMPONENT;
            glType = GL_FLOAT;
        }

        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(m_GLTextureType, m_ID);
        glTexImage2D(m_GLTextureType, 0, sRGBformat, width, height, 0, colorFormat, glType, textureData);
        glGenerateMipmap(m_GLTextureType); // TODO: optional mipmap
        setTextureWrapMode(GL_REPEAT, GL_REPEAT);
        setTextureFilterMode(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    }
    std::string Texture2D::getPath() const
    {
        return m_Path;
    }
    TextureType Texture2D::getType() const
    {
        return m_Type;
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
        return Texture2D(data, width, height, format, type, path);
    }

    Texture2DMultisample::Texture2DMultisample(i32 w, i32 h)
    {
        width = w;
        height = h;
        m_GLTextureType = GL_TEXTURE_2D_MULTISAMPLE;
        colorFormat = GL_RGB;

        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(m_GLTextureType, m_ID);
        glTexImage2DMultisample(m_GLTextureType, 4, colorFormat, width, height, GL_TRUE);
        setTextureWrapMode(GL_REPEAT, GL_REPEAT);
        setTextureFilterMode(GL_LINEAR, GL_LINEAR);
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
    RenderTexture::RenderTexture(i32 w, i32 h)
        : Texture2D(NULL, w, h, GL_RGB, TextureType::RENDER_TEXTURE)
    {
        width = w;
        height = h;

        setTextureWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        setTextureFilterMode(GL_LINEAR, GL_LINEAR);
        m_Framebuffer = Framebuffer(w, h, m_ID);

        m_MultisampleTexture = Texture2DMultisample(width, height);
        m_MultisampleFramebuffer = FramebufferMultisample(width, height, m_MultisampleTexture.getID());
    }
    RenderTexture::~RenderTexture()
    {
    }
    void RenderTexture::bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }
    void RenderTexture::bindMultisample() const
    {
        m_MultisampleFramebuffer.bind();
    }
    void RenderTexture::unbindMultisample() const
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MultisampleFramebuffer.getID());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer.getID());
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        m_MultisampleFramebuffer.unbind();
    }
}