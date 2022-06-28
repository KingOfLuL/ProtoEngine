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
        int width, height;

    public:
        Window() = default;
        Window(const std::string &name, bool fullscreen, int resW, int resH);
        ~Window();

    public:
        void drawToWindow();
        RenderTexture *getWindowRenderTexture() const;
        GLFWwindow *getGLFWwindow() const;

    private:
        GLFWwindow *m_Window;
        Vertexbuffer m_Screen;
        RenderTexture *m_WindowTexture;

    private:
        void sizeCallback(int width, int height);
        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow *window, double xOffset, double yOffset);
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    };
}
#endif // SRC_ENGINE_WINDOW_WINDOW
