#include "libs.hpp"

#include "window.hpp"
#include "engine.hpp"
#include "input/input.hpp"

namespace Engine
{
    Window::Window(const std::string &name, bool fullscreen, int resW, int resH)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        width = mode->width;
        height = mode->height;

        glfwWindowHint(GLFW_DECORATED, !fullscreen);

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

        const Vertex verts[] = {
            Vertex({-1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}),
            Vertex({-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}),
            Vertex({1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}),
            Vertex({1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}),
        };
        const int indices[] = {0, 1, 3, 1, 2, 3};
        m_Screen = Vertexbuffer(&verts[0], 4);
        m_Screen.addIndexbuffer(&indices[0], 6);

        Renderer::shaderScreen = new Shader("vertex/screen.vs.glsl", "fragment/screen.fs.glsl", "Screen");
        m_WindowTexture = new RenderTexture(resW, resH);
    }
    RenderTexture *Window::getWindowRenderTexture() const
    {
        return m_WindowTexture;
    }
    void Window::drawToWindow()
    {
        Renderer::shaderScreen->use();
        glViewport(0, 0, width, height);

        glActiveTexture(GL_TEXTURE0);
        m_WindowTexture->bindTexture();

        glDisable(GL_DEPTH_TEST);
        m_Screen.draw();
        glEnable(GL_DEPTH_TEST);
    }
    Window::~Window()
    {
        delete m_WindowTexture;
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
        Input::internal_updateMouseMovement(xOffset, yOffset);
    }
    void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        (void)window;
        Input::internal_keyCallback(key, scancode, action, mods);
    }
    GLFWwindow *Window::getGLFWwindow() const
    {
        return m_Window;
    }
}