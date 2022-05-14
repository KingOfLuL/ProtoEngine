#define STB_IMAGE_IMPLEMENTATION

#include "engine.hpp"
#include "time/time.hpp"
#include "input/input.hpp"

#include <stb/stb_image.h>

namespace Engine
{
    Scene *activeScene = nullptr;
    Window *activeWindow = nullptr;

    Event onRenderUpdate;

    void init(Scene &scene, const std::string &windowName, int windowW, int windowH)
    {
        activeScene = &scene;

        stbi_set_flip_vertically_on_load(false);
        glfwInit();
        internal_util_init();

        activeWindow = new Window(windowName, windowW, windowH);

        Renderer::init();
        Time::init();
    }
    void run()
    {
        activeScene->start();
        while (!glfwWindowShouldClose(activeWindow->getGLFWwindow()))
        {
            onRenderUpdate.call();
            activeScene->update();

            if (glfwGetKey(activeWindow->getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(activeWindow->getGLFWwindow(), true);

            Renderer::render();

            glfwSwapBuffers(activeWindow->getGLFWwindow());
            glfwPollEvents();
        }
    }
}