#pragma once

struct GLFWwindow;

namespace MyEngine {
    class VertexArray;

    // Класс рендера
    class Render_OpenGL {
    public:
        // Инициализация рендера
        static bool init(GLFWwindow* pWindow);

        // Отрисовка, задание цвета, очистка цвета, задание вида, тест глубины (включение и выключение)
        static void draw(const VertexArray& vertex_array);
        static void set_clear_color(const float r, const float g, const float b, const float a);
        static void clear();
        static void set_viewport(const unsigned int width, const unsigned int height, const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);
        static void enable_depth_test();
        static void disable_depth_test();

        // Вспомогательные функции для рендера
        static const char* get_vendor_str();
        static const char* get_renderer_str();
        static const char* get_version_str();
    };

}