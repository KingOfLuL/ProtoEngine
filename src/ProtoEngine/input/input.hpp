#ifndef SRC_ENGINE_INPUT
#define SRC_ENGINE_INPUT

#include "util/util.hpp"
#include "window/window.hpp"

namespace Engine::Input
{
    extern Event MouseMoveEvent;

    extern glm::vec2 MousePosition;
    extern glm::vec2 MouseMovement;

    bool keyIsDown(i32 key);

    // this function gets called when the mouse is moved
    void internal_updateMouseMovement(float xOff, float yOff);
    void internal_keyCallback(i32 key, i32 scancode, i32 action, i32 mods);
}
namespace Engine::Keys
{
    // a key is an integer from the GLFW library. If a key is not defined here, use the GLFW version (GLFW_KEY_<key>)
    typedef i32 Key;

    const Key Unknown = GLFW_KEY_UNKNOWN;
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
    const Key NumLock = GLFW_KEY_NUM_LOCK;
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
    const Key Numpad_Add = GLFW_KEY_KP_ADD;
    const Key Numpad_Subtract = GLFW_KEY_KP_SUBTRACT;
    const Key Numpad_Multiply = GLFW_KEY_KP_MULTIPLY;
    const Key Numpad_Divide = GLFW_KEY_KP_DIVIDE;
    const Key Numpad_Enter = GLFW_KEY_KP_ENTER;
    const Key Numpad_Decimal = GLFW_KEY_KP_DECIMAL;
    const Key Numpad_Equal = GLFW_KEY_KP_EQUAL;
    const Key ArrowUp = GLFW_KEY_UP;
    const Key ArrowDown = GLFW_KEY_DOWN;
    const Key ArrowLeft = GLFW_KEY_LEFT;
    const Key ArrowRight = GLFW_KEY_RIGHT;
    const Key LeftSuper = GLFW_KEY_LEFT_SUPER;
    const Key RightSuper = GLFW_KEY_RIGHT_SUPER;
    const Key Menu = GLFW_KEY_MENU;
    const Key Home = GLFW_KEY_HOME;
    const Key Escape = GLFW_KEY_ESCAPE;
    const Key End = GLFW_KEY_END;
    const Key Space = GLFW_KEY_SPACE;
    const Key Tab = GLFW_KEY_TAB;
    const Key Backspace = GLFW_KEY_BACKSPACE;
    const Key Enter = GLFW_KEY_ENTER;
    const Key Delete = GLFW_KEY_DELETE;
    const Key LeftCtrl = GLFW_KEY_LEFT_CONTROL;
    const Key RightCtrl = GLFW_KEY_RIGHT_CONTROL;
    const Key LeftShift = GLFW_KEY_LEFT_SHIFT;
    const Key RightShift = GLFW_KEY_RIGHT_SHIFT;
    const Key LeftAlt = GLFW_KEY_LEFT_ALT;
    const Key RightAlt = GLFW_KEY_RIGHT_ALT;
    const Key CapsLock = GLFW_KEY_CAPS_LOCK;
    const Key Period = GLFW_KEY_PERIOD;
    const Key Comma = GLFW_KEY_COMMA;
    const Key Semicolon = GLFW_KEY_SEMICOLON;
    const Key Slash = GLFW_KEY_SLASH;
    const Key Backslash = GLFW_KEY_BACKSLASH;
    const Key Apostrophe = GLFW_KEY_APOSTROPHE;
    const Key GraveAccent = GLFW_KEY_GRAVE_ACCENT;
    const Key Insert = GLFW_KEY_INSERT;
    const Key Equal = GLFW_KEY_EQUAL;
    const Key Minus = GLFW_KEY_MINUS;
    const Key PageUp = GLFW_KEY_PAGE_UP;
    const Key PageDown = GLFW_KEY_PAGE_DOWN;
    const Key Print = GLFW_KEY_PRINT_SCREEN;
    const Key ScrollLock = GLFW_KEY_SCROLL_LOCK;
    const Key Pause = GLFW_KEY_PAUSE;
    const Key LeftBracket = GLFW_KEY_LEFT_BRACKET;
    const Key RightBracket = GLFW_KEY_RIGHT_BRACKET;
    const Key F1 = GLFW_KEY_F1;
    const Key F2 = GLFW_KEY_F2;
    const Key F3 = GLFW_KEY_F3;
    const Key F4 = GLFW_KEY_F4;
    const Key F5 = GLFW_KEY_F5;
    const Key F6 = GLFW_KEY_F6;
    const Key F7 = GLFW_KEY_F7;
    const Key F8 = GLFW_KEY_F8;
    const Key F9 = GLFW_KEY_F9;
    const Key F10 = GLFW_KEY_F10;
    const Key F11 = GLFW_KEY_F11;
    const Key F13 = GLFW_KEY_F13;
    const Key F14 = GLFW_KEY_F14;
    const Key F15 = GLFW_KEY_F15;
    const Key F16 = GLFW_KEY_F16;
    const Key F17 = GLFW_KEY_F17;
    const Key F18 = GLFW_KEY_F18;
    const Key F19 = GLFW_KEY_F19;
    const Key F20 = GLFW_KEY_F20;
    const Key F21 = GLFW_KEY_F21;
    const Key F22 = GLFW_KEY_F22;
    const Key F23 = GLFW_KEY_F23;
    const Key F24 = GLFW_KEY_F24;
    const Key F25 = GLFW_KEY_F25;
}

#endif // SRC_ENGINE_INPUT
