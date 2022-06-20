#include "libs.hpp"

#include "window.hpp"
#include "engine.hpp"
#include "util/vertices.hpp"
#include "input/input.hpp"

namespace Engine
{
    Window::Window(const std::string &name, int w, int h)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        bool fullscreen = mode->width == w && mode->height == h;

        glfwWindowHint(GLFW_DECORATED, !fullscreen);

        if (fullscreen)
        {
            width = mode->width;
            height = mode->height;
        }
        else
        {
            width = w;
            height = h;
        }

        m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
        if (!m_Window)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            return;
        }
        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed initialize glad" << std::endl;
            return;
        }
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(m_Window, mouse_callback);
        glfwSetKeyCallback(m_Window, key_callback);

        glViewport(0, 0, width, height);

        glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

        shader = (new Shader("vertex/screen.vs.glsl", "fragment/screen.fs.glsl", "Screen"));
        screen = Vertexbuffer(&QUAD_VERTICES[0], 4);
        screen.addIndexbuffer(&QUAD_INDICES[0], 6);
    }
    void Window::drawToWindow(const RenderTexture &tex)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        shader->use();
        tex.bindTexture();
        screen.draw();
        glEnable(GL_DEPTH_TEST);
    }
    Window::~Window()
    {
        delete shader;
    }
    void Window::sizeCallback(int width, int height)
    {
        this->width = width;
        this->height = height;
        glViewport(0, 0, this->width, this->height);
    }
    void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        (void)window;
        activeWindow->sizeCallback(width, height);
    }
    void Window::mouse_callback(GLFWwindow *window, double xOffset, double yOffset)
    {
        (void)window;
        Input::internal_engine_updateMouseMovement(xOffset, yOffset);
    }
    void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        (void)window;
        Input::internal_engine_keyCallback(key, scancode, action, mods);
    }
    GLFWwindow *Window::getGLFWwindow() const
    {
        return m_Window;
    }
}