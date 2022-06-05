#ifndef SRC_ENGINE_INPUT
#define SRC_ENGINE_INPUT

#include "util/util.hpp"
#include "window/window.hpp"

namespace Engine::Input
{
    extern Event MouseMoveEvent;

    extern glm::vec2 MousePosition;
    extern glm::vec2 MouseMovement;

    bool keyIsDown(int key);

    // this function gets called when the mouse is moved
    void internal_engine_updateMouseMovement(float xOff, float yOff);
    void internal_engine_keyCallback(int key, int scancode, int action, int mods);
}
namespace Engine::Keys
{
    typedef int Key;

    // a key is an integer from the GLFW library. If a key is not defined here, use the GLFW version (GLFW_KEY_<key>)
    constexpr Key A = GLFW_KEY_A;
    constexpr Key B = GLFW_KEY_B;
    constexpr Key C = GLFW_KEY_C;
    constexpr Key D = GLFW_KEY_D;
    constexpr Key E = GLFW_KEY_E;
    constexpr Key F = GLFW_KEY_F;
    constexpr Key G = GLFW_KEY_G;
    constexpr Key H = GLFW_KEY_H;
    constexpr Key I = GLFW_KEY_I;
    constexpr Key J = GLFW_KEY_J;
    constexpr Key K = GLFW_KEY_K;
    constexpr Key L = GLFW_KEY_L;
    constexpr Key M = GLFW_KEY_M;
    constexpr Key N = GLFW_KEY_N;
    constexpr Key O = GLFW_KEY_O;
    constexpr Key P = GLFW_KEY_P;
    constexpr Key Q = GLFW_KEY_Q;
    constexpr Key R = GLFW_KEY_R;
    constexpr Key S = GLFW_KEY_S;
    constexpr Key T = GLFW_KEY_T;
    constexpr Key U = GLFW_KEY_U;
    constexpr Key V = GLFW_KEY_V;
    constexpr Key W = GLFW_KEY_W;
    constexpr Key X = GLFW_KEY_X;
    constexpr Key Y = GLFW_KEY_Y;
    constexpr Key Z = GLFW_KEY_Z;
    constexpr Key Zero = GLFW_KEY_0;
    constexpr Key One = GLFW_KEY_1;
    constexpr Key Two = GLFW_KEY_2;
    constexpr Key Three = GLFW_KEY_3;
    constexpr Key Four = GLFW_KEY_4;
    constexpr Key Five = GLFW_KEY_5;
    constexpr Key Six = GLFW_KEY_6;
    constexpr Key Seven = GLFW_KEY_7;
    constexpr Key Eight = GLFW_KEY_8;
    constexpr Key Nine = GLFW_KEY_9;
    constexpr Key Numpad_Zero = GLFW_KEY_KP_0;
    constexpr Key Numpad_One = GLFW_KEY_KP_1;
    constexpr Key Numpad_Two = GLFW_KEY_KP_2;
    constexpr Key Numpad_Three = GLFW_KEY_KP_3;
    constexpr Key Numpad_Four = GLFW_KEY_KP_4;
    constexpr Key Numpad_Five = GLFW_KEY_KP_5;
    constexpr Key Numpad_Six = GLFW_KEY_KP_6;
    constexpr Key Numpad_Seven = GLFW_KEY_KP_7;
    constexpr Key Numpad_Eight = GLFW_KEY_KP_8;
    constexpr Key Numpad_Nine = GLFW_KEY_KP_9;
    constexpr Key Space = GLFW_KEY_SPACE;
    constexpr Key Tab = GLFW_KEY_TAB;
    constexpr Key Backspace = GLFW_KEY_BACKSPACE;
    constexpr Key Enter = GLFW_KEY_ENTER;
    constexpr Key Delete = GLFW_KEY_DELETE;
    constexpr Key LeftCtrl = GLFW_KEY_LEFT_CONTROL;
    constexpr Key LeftShift = GLFW_KEY_LEFT_SHIFT;
    constexpr Key LeftAlt = GLFW_KEY_LEFT_ALT;
    constexpr Key CapsLock = GLFW_KEY_CAPS_LOCK;
    constexpr Key RightShift = GLFW_KEY_RIGHT_SHIFT;
    constexpr Key RightCtrl = GLFW_KEY_RIGHT_CONTROL;
    constexpr Key Period = GLFW_KEY_PERIOD;
    constexpr Key Comma = GLFW_KEY_COMMA;
}

#endif // SRC_ENGINE_INPUT
