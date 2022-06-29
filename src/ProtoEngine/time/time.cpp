#include "time.hpp"

#include "engine.hpp"

namespace Engine::Time
{
    f32 deltaTime = 0.f;
    f32 time = 0.f;

    f32 lastFrame = 0;

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