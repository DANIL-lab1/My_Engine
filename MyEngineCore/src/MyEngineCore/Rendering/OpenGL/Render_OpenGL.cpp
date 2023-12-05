#include "Render_OpenGL.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "MyEngineCore/Log.hpp"


namespace MyEngine {

    // Инициализация рендера
    bool Render_OpenGL::init(GLFWwindow* pWindow) {
        glfwMakeContextCurrent(pWindow);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return false;
        }
        LOG_INFO("OpenGL context initialized:");
        LOG_INFO("  Vendor: {0}", get_vendor_str());
        LOG_INFO("  Renderer: {0}", get_renderer_str());
        LOG_INFO("  Version: {0}", get_version_str());
        return true;
    }

    // Отрисовка рендера
    void Render_OpenGL::draw(const VertexArray& vertex_array) {
        vertex_array.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.get_indices_count()), GL_UNSIGNED_INT, nullptr);
    }

    // Задание цвета 
    void Render_OpenGL::set_clear_color(const float r, const float g, const float b, const float a) {
        glClearColor(r, g, b, a);
    }

    // Очистка цвета
    void Render_OpenGL::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Задание вида
    void Render_OpenGL::set_viewport(const unsigned int width, const unsigned int height, 
        const unsigned int left_offset, const unsigned int bottom_offset) {
        glViewport(left_offset, bottom_offset, width, height);
    }

    // Включение теста глубины
    void Render_OpenGL::enable_depth_test(){
        glEnable(GL_DEPTH_TEST);
    }

    // Выключение теста глубины
    void Render_OpenGL::disable_depth_test(){
        glDisable(GL_DEPTH_TEST);
    }

    // Вспомогательные функции для рендера

    const char* Render_OpenGL::get_vendor_str() {
        return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    }

    const char* Render_OpenGL::get_renderer_str() {
        return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    }

    const char* Render_OpenGL::get_version_str() {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }

}