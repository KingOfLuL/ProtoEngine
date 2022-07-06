#include "libs.hpp"

#include "input.hpp"
#include "application/application.hpp"

namespace Engine::Input
{
    Event MouseMoveEvent;

    glm::vec2 MouseMovement(0.f);
    glm::vec2 MousePosition(0.f);

    bool keyIsDown(int key)
    {
        return glfwGetKey(Engine::application->window->getGLFWwindow(), key) == GLFW_PRESS;
    }

    f32 lastX, lastY;
    bool firstMove = true;
    void internal_updateMouseMovement(f32 xOff, f32 yOff)
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
    void internal_keyCallback(i32 key, i32 scancode, i32 action, i32 mods)
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