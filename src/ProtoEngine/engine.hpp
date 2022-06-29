#ifndef SRC_ENGINE_ENGINE
#define SRC_ENGINE_ENGINE

#include "scene/scene.hpp"
#include "window/window.hpp"
#include "util/util.hpp"

namespace Engine
{
    extern Scene *activeScene;
    extern Window *activeWindow;

    // Event that gets called on each frame
    extern Event onRenderUpdate;

    void init(Scene &scene, const std::string &windowName, bool fullscreen);
    void run();
}

#endif // SRC_ENGINE_ENGINE
