#pragma once

struct GLFWwindow;

namespace MyEngine {

    //Класс для графической оболочки
    class UIModule
    {
    public:
        // Метод при создании/закрытии окна, при начале/конце отрисовки
        static void on_window_create(GLFWwindow* pWindow);
        static void on_window_close();
        static void on_ui_draw_begin();
        static void on_ui_draw_end();

        static void ShowExampleAppDockSpace(bool* p_open);
    };

}