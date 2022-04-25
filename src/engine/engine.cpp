#define STB_IMAGE_IMPLEMENTATION

#include "Engine.hpp"
#include "util/util.hpp"
#include "Time/Time.hpp"
#include "Input/Input.hpp"

#include <stb/stb_image.h>

namespace Engine
{
    Scene *activeScene = nullptr;
    Window *activeWindow = nullptr;

    std::vector<std::function<void()>> onUpdateFunctions;

    void addOnUpdate(const std::function<void()> &func)
    {
        onUpdateFunctions.push_back(func);
    }

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
            activeScene->update();
            for (auto func : onUpdateFunctions)
            {
                func();
            }

            if (glfwGetKey(activeWindow->getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(activeWindow->getGLFWwindow(), true);

            // if (keyIsDown(GLFW_KEY_2))
            //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // else
            //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            Renderer::render();

            glfwSwapBuffers(activeWindow->getGLFWwindow());
            glfwPollEvents();
        }
    }
}