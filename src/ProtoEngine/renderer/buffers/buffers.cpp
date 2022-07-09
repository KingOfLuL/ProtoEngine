#include "libs.hpp"

#include "buffers.hpp"

#include "glad/glad.h"
#include "renderer/mesh/mesh.hpp"
#include "renderer/texture/texture.hpp"

namespace Engine
{
    u32 Buffer::getID() const
    {
        return m_ID;
    }

    Indexbuffer::Indexbuffer(const void *data, i32 count) : m_Count(count)
    {
        glGenBuffers(1, &m_ID);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(u32), data, GL_STATIC_DRAW);
    }
    void Indexbuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }
    void Indexbuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    i32 Indexbuffer::getCount() const
    {
        return m_Count;
    }
    void Indexbuffer::deleteBuffers()
    {
        glDeleteBuffers(1, &m_ID);
    }

    Vertexbuffer::Vertexbuffer(const void *data, i32 dataCount) : m_Count(dataCount)
    {
        glGenVertexArrays(1, &m_ID);
        glGenBuffers(1, &m_BufferID);

        bind();

        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ARRAY_BUFFER, dataCount * sizeof(Vertex), data, GL_STATIC_DRAW);

        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));
        // vertex Colors
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

        unbind();
    }
    void Vertexbuffer::addIndexbuffer(const void *indexBufferData, i32 indexBufferDataCount)
    {
        bind();
        m_IndexBuffer = Indexbuffer(indexBufferData, indexBufferDataCount);
        unbind();
        m_HasIndexbuffer = true;
    }
    void Vertexbuffer::bind() const
    {
        glBindVertexArray(m_ID);
    }
    void Vertexbuffer::unbind() const
    {
        glBindVertexArray(0);
    }
    void Vertexbuffer::draw() const
    {
        bind();
        if (m_HasIndexbuffer)
            glDrawElements(GL_TRIANGLES, m_IndexBuffer.getCount(), GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, m_Count);
        unbind();
    }
    i32 Vertexbuffer::getIndicesCount() const
    {
        return m_IndexBuffer.getCount();
    }
    void Vertexbuffer::deleteBuffers()
    {
        glDeleteBuffers(1, &m_ID);
        glDeleteBuffers(1, &m_BufferID);
        m_IndexBuffer.deleteBuffers();
    }
    void Vertexbuffer::setData(const void *data, i32 dataCount)
    {
        m_Count = dataCount;

        glBindVertexArray(m_ID);

        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(Vertex), data, GL_STATIC_DRAW);
    }

    Renderbuffer::Renderbuffer(i32 w, i32 h, GLenum storageType, GLenum attachment, bool useAntiAliasing)
        : width(w), height(h), m_StorageType(storageType), m_AntiAliasing(useAntiAliasing)
    {
        glGenRenderbuffers(1, &m_ID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_ID);

        if (m_AntiAliasing)
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, m_StorageType, width, height);
        else
            glRenderbufferStorage(GL_RENDERBUFFER, m_StorageType, width, height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_ID);
    }

    Framebuffer::Framebuffer(i32 w, i32 h, u32 textureID, bool useAntiAliasing)
        : width(w), height(h), m_AntiAliasing(useAntiAliasing)
    {
        glGenFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_AntiAliasing ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, textureID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr
                << "ERROR: Framebuffer is not complete" << std::endl;
    }
    void Framebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    }
    void Framebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Uniformbuffer::Uniformbuffer(u32 size, u32 bindingPoint)
        : m_Size(size), m_BindingPoint(bindingPoint)
    {
        glGenBuffers(1, &m_ID);
        bind();
        glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_ID, 0, size);
        unbind();
    }
    void Uniformbuffer::setData(const void *data, u32 size, u32 offset)
    {
        bind();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        unbind();
    }
    void Uniformbuffer::bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
    }
    void Uniformbuffer::unbind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}
