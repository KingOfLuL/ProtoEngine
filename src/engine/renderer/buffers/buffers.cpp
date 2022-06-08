#include "libs.hpp"

#include "buffers.hpp"

#include "glad/glad.h"
#include "renderer/mesh/mesh.hpp"
#include "renderer/texture/texture.hpp"

namespace Engine
{
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

        glBindVertexArray(m_ID);

        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ARRAY_BUFFER, dataCount * sizeof(Vertex), data, GL_STATIC_DRAW);

        // // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        // // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        // // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
        // // vertex Colors
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

        glBindVertexArray(0);
    }
    void Vertexbuffer::addIndexbuffer(const void *indexBufferData, int indexBufferDataCount)
    {
        glBindVertexArray(m_ID);
        m_IndexBuffer = Indexbuffer(indexBufferData, indexBufferDataCount);
        glBindVertexArray(0);
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
            glDrawElements(GL_TRIANGLES, getIndicesCount(), GL_UNSIGNED_INT, 0);
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
        glBindVertexArray(m_ID);

        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ARRAY_BUFFER, dataCount * sizeof(Vertex), data, GL_STATIC_DRAW);
    }

    Framebuffer::Framebuffer(int w, int h)
    {
        glGenFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        glGenTextures(1, &m_Colorbuffer);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Colorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Colorbuffer, 0);

        glGenRenderbuffers(1, &m_Renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Renderbuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR: Framebuffer is not complete" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void Framebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        glEnable(GL_DEPTH_TEST);
    }
    void Framebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
    }
    void Framebuffer::bindTextureBuffer() const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Colorbuffer);
    }

    Uniformbuffer::Uniformbuffer(uint32_t size, uint32_t bindingPoint)
    {
        glGenBuffers(1, &m_ID);
        bind();
        glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_ID, 0, size);
        unbind();
    }
    void Uniformbuffer::addBindingPoint(uint32_t point, uint32_t size, uint32_t offset)
    {
        bind();
        glBindBufferRange(GL_UNIFORM_BUFFER, point, m_ID, offset, size);
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
