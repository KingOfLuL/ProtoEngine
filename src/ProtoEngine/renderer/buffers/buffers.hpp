#ifndef SRC_ENGINE_RENDERER_BUFFERS_BUFFERS
#define SRC_ENGINE_RENDERER_BUFFERS_BUFFERS

#include <map>
#include "glad/glad.h"

namespace Engine
{
    class Buffer
    {
    public:
        virtual void bind() const {}
        virtual void unbind() const {}

        uint32_t getID() const;

    protected:
        uint32_t m_ID;
    };

    class Indexbuffer : public Buffer
    {
    public:
        Indexbuffer() = default;
        Indexbuffer(const void *data, int dataCount);
        void deleteBuffers();

        void bind() const override;
        void unbind() const override;
        int getCount() const;

    private:
        int m_Count;
    };

    class Vertexbuffer : public Buffer
    {
    public:
        Vertexbuffer() = default;
        Vertexbuffer(const void *data, int dataCount);

    public:
        void addIndexbuffer(const void *indexBufferData, int indexBufferDataCount);
        void deleteBuffers();
        void setData(const void *data, int dataCount);
        void bind() const override;
        void unbind() const override;
        void draw() const;
        int getIndicesCount() const;

    private:
        uint32_t m_BufferID;
        Indexbuffer m_IndexBuffer;
        bool m_HasIndexbuffer = false;
        int m_Count;
    };

    class Renderbuffer : public Buffer
    {
    public:
        Renderbuffer() = default;
        Renderbuffer(int w, int h, GLenum storageType);

    public:
        int width;
        int height;

    private:
        GLenum m_StorageType;
    };

    class RenderTexture;
    class Framebuffer : public Buffer
    {
    public:
        Framebuffer() = default;
        Framebuffer(int w, int h, RenderTexture *renderTexture);

        void bind() const override;
        void unbind() const override;

    public:
        int width;
        int height;

    private:
        Renderbuffer m_Renderbuffer;
        RenderTexture *m_RenderTexture;
    };

    class Uniformbuffer : public Buffer
    {
    public:
        Uniformbuffer() = default;
        Uniformbuffer(uint32_t size, uint32_t bindingPoint);

        void setData(const void *data, uint32_t size, uint32_t offset);

        void bind() const override;
        void unbind() const override;

    private:
        uint32_t m_Size;
        uint32_t m_BindingPoint;
    };
}
#endif // SRC_ENGINE_RENDERER_BUFFERS_BUFFERS
