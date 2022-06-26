#include "libs.hpp"

#include "buffers.hpp"

#include "glad/glad.h"
#include "renderer/mesh/mesh.hpp"
#include "renderer/texture/texture.hpp"

namespace Engine
{
    uint32_t Buffer::getID() const
    {
        return m_ID;
    }

    Indexbuffer::Indexbuffer(const void *data, int count) : m_Count(count)
    {
        glGenBuffers(1, &m_ID);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    }
    void Indexbuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }
    void Indexbuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    int Indexbuffer::getCount() const
    {
        return m_Count;
    }
    void Indexbuffer::deleteBuffers()
    {
        glDeleteBuffers(1, &m_ID);
    }

    Vertexbuffer::Vertexbuffer(const void *data, int dataCount) : m_Count(dataCount)
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
        // vertex Colors
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

        unbind();
    }
    void Vertexbuffer::addIndexbuffer(const void *indexBufferData, int indexBufferDataCount)
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
    int Vertexbuffer::getIndicesCount() const
    {
        return m_IndexBuffer.getCount();
    }
    void Vertexbuffer::deleteBuffers()
    {
        glDeleteBuffers(1, &m_ID);
        glDeleteBuffers(1, &m_BufferID);
        m_IndexBuffer.deleteBuffers();
    }
    void Vertexbuffer::setData(const void *data, int dataCount)
    {
        m_Count = dataCount;

        glBindVertexArray(m_ID);

        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(Vertex), data, GL_STATIC_DRAW);
    }

    Renderbuffer::Renderbuffer(int w, int h, GLenum storageType)
        : width(w), height(h), m_StorageType(storageType)
    {
        glGenRenderbuffers(1, &m_ID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
        glRenderbufferStorage(GL_RENDERBUFFER, m_StorageType, width, height);
    }

    Framebuffer::Framebuffer(int w, int h, RenderTexture *renderTexture)
        : width(w), height(h), m_RenderTexture(renderTexture)
    {
        glGenFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTexture->getTexture()->getID(), 0);

        m_Renderbuffer = Renderbuffer(width, height, GL_DEPTH24_STENCIL8);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Renderbuffer.getID());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR: Framebuffer is not complete" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void Framebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    }
    void Framebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Uniformbuffer::Uniformbuffer(uint32_t size, uint32_t bindingPoint)
        : m_Size(size), m_BindingPoint(bindingPoint)
    {
        glGenBuffers(1, &m_ID);
        bind();
        glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_ID, 0, size);
        unbind();
    }
    void Uniformbuffer::setData(const void *data, uint32_t size, uint32_t offset)
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