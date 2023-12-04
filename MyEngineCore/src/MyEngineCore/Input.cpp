#include "MyEngineCore/Input.hpp"

namespace MyEngine {
    // Массив клавиш и мышки
    bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
    bool Input::m_mouse_buttons_pressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};

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

    // Нажата ли мышка
    bool Input::IsMouseButtonPressed(const MouseButton mouse_button){
        return m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)];
    }

    // Функция при нажатии мышки
    void Input::PressMouseButton(const MouseButton mouse_button){
        m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = true;
    }

    // Функция при отпускании мышки
    void Input::ReleaseMouseButton(const MouseButton mouse_button){
        m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = false;
    }
}