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

        u32 getID() const;

    protected:
        u32 m_ID;
    };

    class Indexbuffer : public Buffer
    {
    public:
        Indexbuffer() = default;
        Indexbuffer(const void *data, i32 dataCount);
        void deleteBuffers();

        void bind() const override;
        void unbind() const override;
        i32 getCount() const;

    private:
        i32 m_Count;
    };

    class Vertexbuffer : public Buffer
    {
    public:
        Vertexbuffer() = default;
        Vertexbuffer(const void *data, i32 dataCount);

    public:
        void addIndexbuffer(const void *indexBufferData, i32 indexBufferDataCount);
        void deleteBuffers();
        void setData(const void *data, i32 dataCount);
        void bind() const override;
        void unbind() const override;
        void draw() const;
        i32 getIndicesCount() const;

    private:
        u32 m_BufferID;
        Indexbuffer m_IndexBuffer;
        bool m_HasIndexbuffer = false;
        i32 m_Count;
    };

    class Renderbuffer : public Buffer
    {
    public:
        Renderbuffer() = default;
        Renderbuffer(i32 w, i32 h, GLenum storageType, GLenum attachment, bool useAntiAliasing = true);

    public:
        i32 width;
        i32 height;

    private:
        GLenum m_StorageType;
        bool m_AntiAliasing;
    };

    class Framebuffer : public Buffer
    {
    public:
        Framebuffer() = default;
        Framebuffer(i32 w, i32 h, u32 textureID, bool useAntiAliasing = true);

        void bind() const override;
        void unbind() const override;

    public:
        i32 width;
        i32 height;

    private:
        bool m_AntiAliasing;
        Renderbuffer m_Renderbuffer;
    };

    class Uniformbuffer : public Buffer
    {
    public:
        Uniformbuffer() = default;
        Uniformbuffer(u32 size, u32 bindingPoint);

        void setData(const void *data, u32 size, u32 offset);

        void bind() const override;
        void unbind() const override;

    private:
        u32 m_Size;
        u32 m_BindingPoint;
    };
}
#endif // SRC_ENGINE_RENDERER_BUFFERS_BUFFERS
