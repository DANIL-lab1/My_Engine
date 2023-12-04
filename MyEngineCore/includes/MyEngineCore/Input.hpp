#pragma once

#include "Keys.hpp"

namespace MyEngine {

    // Класс ввода
    class Input {
    public:
        // Нажата ли клавиша и две функции (нажатие и отпускание)
        static bool IsKeyPressed(const KeyCode key_code);
        static void PressKey(const KeyCode key_code);
        static void ReleaseKey(const KeyCode key_code);

        // Нажата ли мышка и две функции (нажатие и отпукание)
        static bool IsMouseButtonPressed(const MouseButton mouse_button);
        static void PressMouseButton(const MouseButton mouse_button);
        static void ReleaseMouseButton(const MouseButton mouse_button);

    private:
        // Массив клавиш и мышки
        static bool m_keys_pressed[];
        static bool m_mouse_buttons_pressed[];
    };
}