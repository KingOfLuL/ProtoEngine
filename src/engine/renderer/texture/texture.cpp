#include "libs.hpp"

#include "Texture.hpp"

#include "glad/glad.h"
#include "util/util.hpp"

#include <stb/stb_image.h>

namespace Engine
{
    std::vector<Texture2D *> loadedTextures;

    void Texture::setParameter(GLenum parameter, GLenum value)
    {
        glTexParameteri(GL_TEXTURE_2D, parameter, value);
    }
    void Texture::bind() const
    {
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    Texture2D::Texture2D(uint32_t id, int w, int h, GLenum colorFormat, const TextureType &type, const std::string &path) : m_Type(type), m_Path(path)
    {
        ID = id;
        width = w;
        height = h;
        colorFormat = colorFormat;
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
        glGenTextures(1, &ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

        int w, h, nrChannels;

        for (size_t i = 0; i < faces.size(); i++)
        {
            auto path = PathUtil::FULL_PATH + PathUtil::TEXTURE_PATH + faces[i];
            unsigned char *data = stbi_load(path.c_str(), &w, &h, &nrChannels, 0);
            if (data)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else
                std::cout << "failed to load Cubemap at: " << faces[i] << std::endl;
            stbi_image_free(data);
        }

        setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        colorFormat = GL_RGB;
    }
    void Cubemap::setParameter(GLenum parameter, GLenum value)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, parameter, value);
    }
    void Cubemap::bind() const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    }

    void loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType typeName, std::vector<Texture2D> &textures, std::vector<Texture2D> &loadedTextures)
    {
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString texPath;
            mat->GetTexture(type, i, &texPath);

            std::string path = PathUtil::toUnixStylePath(texPath.C_Str());
            path = PathUtil::getRelativePath(path, PathUtil::TEXTURE_PATH);

            // check if texture has already been loaded
            for (auto tex : loadedTextures)
            {
                if (tex.getPath() == path)
                    return;
            }
            Texture2D texture = loadTextureFromFile(path, typeName);
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }
    void loadMaterialTexture(const std::string &path, TextureType texType, Material *material)
    {
        for (Texture2D *tex : loadedTextures)
            if (tex->getPath() == path)
            {
                material->textures.push_back(tex);
                return;
            }
        Texture2D *tex = new Texture2D(loadTextureFromFile(path, texType)); // FIXME: No free
        material->textures.push_back(tex);
        loadedTextures.push_back(tex);
    }

    Texture2D loadTextureFromFile(const std::string &path, TextureType type)
    {
        std::string fileName = PathUtil::FULL_PATH + PathUtil::TEXTURE_PATH + path;

        uint32_t textureID;

        int width, height, nrComponents;
        unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
        GLenum format = GL_RGBA;
        if (data)
        {
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            createGLTexture(textureID, width, height, format, data, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, true);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << fileName << std::endl;
        }
        stbi_image_free(data);

        return Texture2D(textureID, width, height, format, type, path);
    }
    void createGLTexture(uint32_t &id, int w, int h, GLenum colorFormat, const void *data, GLenum minFilter, GLenum maxFilter, GLenum wrapS, GLenum wrapT, bool mipmap)
    {
        glGenTextures(1, &id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, w, h, 0, colorFormat, GL_UNSIGNED_BYTE, data);
        if (mipmap)
            glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
    }

    RenderTexture::RenderTexture(int w, int h) : m_Framebuffer(w, h)
    {
        width = w;
        height = h;
        colorFormat = GL_RGB;
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
        m_Framebuffer.bindTextureBuffer();
    }
}