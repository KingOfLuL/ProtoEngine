#include "libs.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "engine.hpp"
#include "time/time.hpp"
#include "input/input.hpp"
#include "renderer/material/shader/shader.hpp"

#include <stb/stb_image.h>

namespace Engine
{
    Scene *activeScene = nullptr;
    Window *activeWindow = nullptr;

    Event onRenderUpdate;

    void init(Scene &scene, const std::string &windowName, bool fullscreen, int resolutionWidth, int resolutionHeight)
    {
        activeScene = &scene;

        stbi_set_flip_vertically_on_load(false);
        internal_util_init();

        activeWindow = new Window(windowName, fullscreen, resolutionWidth, resolutionHeight);

        Renderer::init();
        Time::init();
    }
    void run()
    {
        activeScene->start();
        while (!glfwWindowShouldClose(activeWindow->getGLFWwindow()))
        {
            if (glfwGetKey(activeWindow->getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(activeWindow->getGLFWwindow(), true);
                break;
            }

            onRenderUpdate.call();
            activeScene->update();

            Renderer::render();

            activeScene->lateUpdate();

            glfwSwapBuffers(activeWindow->getGLFWwindow());
            glfwPollEvents();
        }
    }
}