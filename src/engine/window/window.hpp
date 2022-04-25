#ifndef SRC_ENGINE_WINDOW_WINDOW
#define SRC_ENGINE_WINDOW_WINDOW

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "renderer/texture/texture.hpp"
#include "renderer/material/shader/shader.hpp"

namespace Engine
{
    class Window
    {
    public:
        int width, height;
        Shader shader;
        Vertexbuffer screen;

    public:
        Window() = default;
        Window(const std::string &name, int w, int h);
        GLFWwindow *getGLFWwindow() const;

    private:
        GLFWwindow *m_Window;

    private:
        void sizeCallback(int width, int height);
        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow *window, double xOffset, double yOffset);
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    };
}
#endif // SRC_ENGINE_WINDOW_WINDOW
