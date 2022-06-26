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
    void internal_updateMouseMovement(float xOff, float yOff);
    void internal_keyCallback(int key, int scancode, int action, int mods);
}
namespace Engine::Keys
{
    // a key is an integer from the GLFW library. If a key is not defined here, use the GLFW version (GLFW_KEY_<key>)
    typedef int Key;

    const Key A = GLFW_KEY_A;
    const Key B = GLFW_KEY_B;
    const Key C = GLFW_KEY_C;
    const Key D = GLFW_KEY_D;
    const Key E = GLFW_KEY_E;
    const Key F = GLFW_KEY_F;
    const Key G = GLFW_KEY_G;
    const Key H = GLFW_KEY_H;
    const Key I = GLFW_KEY_I;
    const Key J = GLFW_KEY_J;
    const Key K = GLFW_KEY_K;
    const Key L = GLFW_KEY_L;
    const Key M = GLFW_KEY_M;
    const Key N = GLFW_KEY_N;
    const Key O = GLFW_KEY_O;
    const Key P = GLFW_KEY_P;
    const Key Q = GLFW_KEY_Q;
    const Key R = GLFW_KEY_R;
    const Key S = GLFW_KEY_S;
    const Key T = GLFW_KEY_T;
    const Key U = GLFW_KEY_U;
    const Key V = GLFW_KEY_V;
    const Key W = GLFW_KEY_W;
    const Key X = GLFW_KEY_X;
    const Key Y = GLFW_KEY_Y;
    const Key Z = GLFW_KEY_Z;
    const Key Zero = GLFW_KEY_0;
    const Key One = GLFW_KEY_1;
    const Key Two = GLFW_KEY_2;
    const Key Three = GLFW_KEY_3;
    const Key Four = GLFW_KEY_4;
    const Key Five = GLFW_KEY_5;
    const Key Six = GLFW_KEY_6;
    const Key Seven = GLFW_KEY_7;
    const Key Eight = GLFW_KEY_8;
    const Key Nine = GLFW_KEY_9;
    const Key Numpad_Zero = GLFW_KEY_KP_0;
    const Key Numpad_One = GLFW_KEY_KP_1;
    const Key Numpad_Two = GLFW_KEY_KP_2;
    const Key Numpad_Three = GLFW_KEY_KP_3;
    const Key Numpad_Four = GLFW_KEY_KP_4;
    const Key Numpad_Five = GLFW_KEY_KP_5;
    const Key Numpad_Six = GLFW_KEY_KP_6;
    const Key Numpad_Seven = GLFW_KEY_KP_7;
    const Key Numpad_Eight = GLFW_KEY_KP_8;
    const Key Numpad_Nine = GLFW_KEY_KP_9;
    const Key Space = GLFW_KEY_SPACE;
    const Key Tab = GLFW_KEY_TAB;
    const Key Backspace = GLFW_KEY_BACKSPACE;
    const Key Enter = GLFW_KEY_ENTER;
    const Key Delete = GLFW_KEY_DELETE;
    const Key LeftCtrl = GLFW_KEY_LEFT_CONTROL;
    const Key LeftShift = GLFW_KEY_LEFT_SHIFT;
    const Key LeftAlt = GLFW_KEY_LEFT_ALT;
    const Key CapsLock = GLFW_KEY_CAPS_LOCK;
    const Key RightShift = GLFW_KEY_RIGHT_SHIFT;
    const Key RightCtrl = GLFW_KEY_RIGHT_CONTROL;
    const Key Period = GLFW_KEY_PERIOD;
    const Key Comma = GLFW_KEY_COMMA;
}

#endif // SRC_ENGINE_INPUT
