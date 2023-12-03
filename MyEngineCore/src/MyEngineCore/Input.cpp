#include "MyEngineCore/Input.hpp"

namespace MyEngine {
    bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST)] = {};

    // Нажата ли клавиша
    bool Input::IsKeyPressed(const KeyCode key_code) {
        return m_keys_pressed[static_cast<size_t>(key_code)];
    }

    // Функция при нажатии клавиши
    void Input::PressKey(const KeyCode key_code) {
        m_keys_pressed[static_cast<size_t>(key_code)] = true;
    }

    // Функция при отпускании клавиши
    void Input::ReleaseKey(const KeyCode key_code) {
        m_keys_pressed[static_cast<size_t>(key_code)] = false;
    }
}