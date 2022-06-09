#include "time.hpp"

#include "engine.hpp"

namespace Engine::Time
{
    float deltaTime = 0.f;
    float time = 0.f;

    float lastFrame = 0;
    void init()
    {
        Engine::onRenderUpdate += []()
        {
            time = glfwGetTime();
            deltaTime = time - lastFrame;
            lastFrame = time;
        };
    }
}