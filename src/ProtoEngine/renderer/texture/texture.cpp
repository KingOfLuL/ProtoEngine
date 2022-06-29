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

        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(m_GLTextureType, m_ID);
        glTexImage2D(m_GLTextureType, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(m_GLTextureType);
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

    Cubemap::Cubemap(const std::array<std::string, 6> &faces) : m_FacePaths(faces)
    {
        m_GLTextureType = GL_TEXTURE_CUBE_MAP;

        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

        i32 w, h, nrChannels;
        colorFormat = GL_RGB;

        for (size_t i = 0; i < faces.size(); i++)
        {
            auto path = PathUtil::FULL_PATH + PathUtil::TEXTURE_PATH + faces[i];
            void *data = stbi_load(path.c_str(), &w, &h, &nrChannels, 0);
            if (data)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
    {
        m_Texture = new Texture2D(NULL, w, h, GL_RGB);
        m_Texture->setTextureWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        m_Texture->setTextureFilterMode(GL_LINEAR, GL_LINEAR);

        m_Framebuffer = Framebuffer(w, h, this);
    }
    RenderTexture::~RenderTexture()
    {
        delete m_Texture;
    }
    i32 RenderTexture::getWidth() const
    {
        return m_Texture->width;
    }
    i32 RenderTexture::getHeight() const
    {
        return m_Texture->height;
    }
    Texture2D *RenderTexture::getTexture() const
    {
        return m_Texture;
    }
    void RenderTexture::bindFramebuffer() const
    {
        m_Framebuffer.bind();
    }
    void RenderTexture::unbindFramebuffer() const
    {
        m_Framebuffer.unbind();
    }
    void RenderTexture::bindTexture() const
    {
        m_Texture->bind();
    }
}