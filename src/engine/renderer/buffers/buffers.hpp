#ifndef SRC_ENGINE_RENDERER_BUFFERS_BUFFERS
#define SRC_ENGINE_RENDERER_BUFFERS_BUFFERS

#include <map>

namespace Engine
{
    class Indexbuffer
    {
    public:
        Indexbuffer() = default;
        inline Indexbuffer(const void *data, int count);
        void deleteBuffers();

        void bind() const;
        void unbind() const;
        int getCount() const;

    private:
        uint32_t m_ID;
        int m_Count;
    };

    class Vertexbuffer
    {
    public:
        Vertexbuffer() = default;
        Vertexbuffer(const void *data, int dataCount);
        void addIndexbuffer(const void *indexBufferData, int indexBufferDataCount);
        void deleteBuffers();

        void setData(const void *data, int dataCount);

        void bind() const;
        void unbind() const;
        void draw() const;
        int getIndicesCount() const;

    private:
        uint32_t m_ID;
        uint32_t m_BufferID;
        Indexbuffer m_IndexBuffer;
        bool m_HasIndexbuffer = false;
        int m_Count;
    };

    class Framebuffer
    {
    public:
        Framebuffer() = default;
        Framebuffer(int w, int h);

        void bind() const;
        void unbind() const;
        void bindTextureBuffer() const;

    private:
        uint32_t m_ID;
        uint32_t m_Renderbuffer;
        uint32_t m_Texturecolorbuffer;
    };

    class Uniformbuffer
    {
    public:
        Uniformbuffer() = default;
        Uniformbuffer(uint32_t size, uint32_t bindingPoint);

        void setData(const void *data, uint32_t size, uint32_t offset);
        void addBindingPoint(uint32_t point, uint32_t size, uint32_t offset);

        void bind() const;
        void unbind() const;

    private:
        uint32_t m_ID;
    };
}
#endif // SRC_ENGINE_RENDERER_BUFFERS_BUFFERS
