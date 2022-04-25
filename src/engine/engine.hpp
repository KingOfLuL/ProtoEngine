#ifndef SRC_ENGINE_ENGINE
#define SRC_ENGINE_ENGINE

#include "scene/scene.hpp"
#include "window/window.hpp"

namespace Engine
{
    extern Scene *activeScene;
    extern Window *activeWindow;

    /// set a function to be called every frame
    void addOnUpdate(const std::function<void()> &func);
    void init(Scene &scene, const std::string &windowName, int windowW, int windowH);
    void run();
}

#endif // SRC_ENGINE_ENGINE
