//
// EPITECH PROJECT, 2025
// r-type
// File description:
// keys
//

#pragma once

#include <optional>

namespace utils
{
    enum class KeyEvent
    {
        KEY_UP,
        KEY_DOWN
    };

    enum class Keys
    {
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,
        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        SPECIAL_KEY_ENTER,
        SPECIAL_KEY_SPACE,
        SPECIAL_KEY_BACKSPACE,
        ARROW_UP,
        ARROW_LEFT,
        ARROW_RIGHT,
        ARROW_DOWN,
    };

    enum class EventType
    {
        UNKNOWN,
        KEYBOARD,
        MOUSE,
        CLOSE
    };

    struct KeyboardEvent
    {
            KeyEvent type;
            Keys key;
    };

    struct EventManager
    {
            EventType type = EventType::UNKNOWN;
            std::optional<struct KeyboardEvent> KeyboardEvent;
    };

} // namespace utils
