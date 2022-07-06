#include "libs.hpp"
#include "application.hpp"
#include "util/util.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Engine
{
    Application *application;

    Application::Application(const std::string &name, bool fullscreen)
        : name(name)
    {
        stbi_set_flip_vertically_on_load(false);
        internal_util_init();

        window = new Window(name, fullscreen);

        Renderer::init();

        (new Shader("vertex/vertex.vs.glsl", "fragment/lit.fs.glsl", "Lit"));
        (new Shader("vertex/cubemap.vs.glsl", "fragment/cubemap.fs.glsl", "Cubemap"));
        (new Shader("vertex/vertex.vs.glsl", "fragment/bounds.fs.glsl", "Bounds"))
            ->addGeometryShader("geometry/bounds.gs.glsl");

        application = this;
    }
    void Application::run()
    {
        scene->start();
        while (!glfwWindowShouldClose(window->getGLFWwindow()))
        {
            if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window->getGLFWwindow(), true);
                break;
            }

            scene->update();

            Renderer::render();

            scene->lateUpdate();

            glfwSwapBuffers(window->getGLFWwindow());
            glfwPollEvents();
        }
    }
    void Application::setActiveScene(Scene *scene)
    {
        this->scene = scene;
    }
}