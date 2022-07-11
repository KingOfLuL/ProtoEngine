#include "libs.hpp"

#include "window.hpp"
#include "input/input.hpp"
#include "application/application.hpp"

namespace Engine
{
    Window::Window(const std::string &name, bool fullscreen)
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
            Vertex({-1, 1, 0}, {1, 0, 0}, {0, 1}),
            Vertex({-1, -1, 0}, {1, 0, 0}, {0, 0}),
            Vertex({1, -1, 0}, {1, 0, 0}, {1, 0}),
            Vertex({1, 1, 0}, {1, 0, 0}, {1, 1}),
        };
        const u32 indices[] = {0, 1, 3, 1, 2, 3};
        m_Screen = Vertexbuffer(&verts[0], 4);
        m_Screen.addIndexbuffer(&indices[0], 6);

        m_Shader = Shader("vertex/screen.vs.glsl", "fragment/screen.fs.glsl", "Screen");
    }
    void Window::drawToWindow(RenderTexture *texture)
    {
        glViewport(0, 0, width, height);
        m_Shader.use();

        m_Shader.setInt("_DepthTexture", 0);
        m_Shader.setInt("_ShadowMap", 1);
        m_Shader.setInt("_RenderTexture", 2);

        glActiveTexture(GL_TEXTURE0);
        application->scene->mainCamera->renderTarget->getTexture(TextureType::DEPTH_TEXTURE)->bind();
        glActiveTexture(GL_TEXTURE1);
        application->scene->getDirectionalLights()[0]->getCamera()->renderTarget->getTexture(TextureType::DEPTH_TEXTURE)->bind();
        glActiveTexture(GL_TEXTURE2);
        texture->bindColorTexture();

        glDisable(GL_DEPTH_TEST);
        m_Screen.draw();
        glEnable(GL_DEPTH_TEST);
    }
    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
    }
    void Window::sizeCallback(i32 width, i32 height)
    {
        this->width = width;
        this->height = height;
        glViewport(0, 0, this->width, this->height);
    }
    void Window::framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height)
    {
        (void)window;
        application->window->sizeCallback(width, height);
    }
    void Window::mouse_callback(GLFWwindow *window, f64 xOffset, f64 yOffset)
    {
        (void)window;
        Input::internal_updateMouseMovement(xOffset, yOffset);
    }
    void Window::key_callback(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods)
    {
        (void)window;
        Input::internal_keyCallback(key, scancode, action, mods);
    }
    GLFWwindow *Window::getGLFWwindow() const
    {
        return m_Window;
    }
}