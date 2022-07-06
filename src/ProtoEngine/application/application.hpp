#ifndef SRC_PROTOENGINE_APPLICATION_APPLICATION
#define SRC_PROTOENGINE_APPLICATION_APPLICATION

#include "window/window.hpp"
#include "scene/scene.hpp"

namespace Engine
{
    class Application
    {
    public:
        Application(const std::string &name, bool fullscreen);

    public:
        std::string name;
        Window *window;
        Scene *scene = nullptr;

    public:
        void run();
        void setActiveScene(Scene *scene);
    };

    extern Application *application;
}

#endif // SRC_PROTOENGINE_APPLICATION_APPLICATION
