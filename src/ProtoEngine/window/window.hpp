#ifndef SRC_ENGINE_WINDOW_WINDOW
#define SRC_ENGINE_WINDOW_WINDOW

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "entity/components.hpp"
#include "renderer/texture/texture.hpp"
#include "renderer/material/shader/shader.hpp"

namespace Engine
{
    class Window
    {
    public:
        i32 width, height;

    public:
        Window() = default;
        Window(const std::string &name, bool fullscreen);
        ~Window();

    public:
        void drawToWindow(RenderTexture *texture);
        GLFWwindow *getGLFWwindow() const;

    private:
        GLFWwindow *m_Window;
        Vertexbuffer m_Screen;

    private:
        void sizeCallback(i32 width, i32 height);
        static void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height);
        static void mouse_callback(GLFWwindow *window, f64 xOffset, f64 yOffset);
        static void key_callback(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods);
    };
}
#endif // SRC_ENGINE_WINDOW_WINDOW
