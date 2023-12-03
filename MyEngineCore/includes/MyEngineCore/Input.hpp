#pragma once

#include "Keys.hpp"

namespace MyEngine {

    // Класс ввода
    class Input {
    public:
        // Нажата ли клавиша и две функции (нажатие и отпускание клавиши)
        static bool IsKeyPressed(const KeyCode key_code);
        static void PressKey(const KeyCode key_code);
        static void ReleaseKey(const KeyCode key_code);

    private:
        // Массив со всеми клавишами
        static bool m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST)];
    };
}