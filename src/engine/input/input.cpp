#include "libs.hpp"

#include "input.hpp"

#include "engine.hpp"

namespace Engine::Input
{
    Event MouseMoveEvent;

    glm::vec2 MouseMovement(0.f);
    glm::vec2 MousePosition(0.f);

    bool keyIsDown(int key)
    {
        return glfwGetKey(Engine::activeWindow->getGLFWwindow(), key) == GLFW_PRESS;
    }

    float lastX, lastY;
    bool firstMove = true;
    void engine_updateMouseMovement(float xOff, float yOff)
    {
        if (firstMove)
        {
            lastX = xOff;
            lastY = yOff;
            firstMove = false;
        }

        MouseMovement.x = lastX - xOff;
        MouseMovement.y = lastY - yOff;

        MousePosition.x = xOff;
        MousePosition.y = yOff;

        lastX = xOff;
        lastY = yOff;

        MouseMoveEvent.call();
    }
    void engine_keyCallback(int key, int scancode, int action, int mods)
    {
        (void)key;
        (void)scancode;
        (void)action;
        (void)mods;
        // if (action == GLFW_PRESS)
        //     std::cout << "GLFW_PRESS" << std::endl; // key gets pressed
        // if (action == GLFW_RELEASE)
        //     std::cout << "GLFW_RELEASE" << std::endl; // key gets released
        // if (action == GLFW_REPEAT)
        //     std::cout << "GLFW_REPEAT" << std::endl; // key is being pressed
    }
}