// Файл для работы с окном игрового движка

#include "MyEngineCore/Window.hpp"
#include "MyEngineCore/Log.hpp"
#include "MyEngineCore/Modules/UIModule.hpp"

#include "MyEngineCore/Rendering/OpenGL/Render_OpenGL.hpp"
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <imgui/backends/imgui_impl_glfw.h>

namespace MyEngine {   

    // Конструктор и запуск игрового движка
	Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_data({ std::move(title), width, height }) {
		int resultCode = init();
	}

    // Деструктор и закрытие игрового движка
	Window::~Window() {
		shutdown();
	}

    // Иницалиазация игрового движка
    int Window::init() {

        // Вывод данных о игровом движке: название, ширина и высота
        LOG_INFO("Creating window '{0}' width size {1}x{2}!", m_data.title, m_data.width, m_data.height);

        // Далее идут мини-обработка ошибок

        // Ошибка вызова рендера
        glfwSetErrorCallback([](int error_code, const char* description) {
            LOG_CRITICAL("GLFW error: {0}", description);
            });

        if (!glfwInit())
        {
            LOG_CRITICAL("Can't initialize GLFW!");
            return -1;
        }


        // Создание контекста для обработки ошибок
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        // Ошибка чтения данных
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow){
            LOG_CRITICAL("Can't create window {0} width size {1}x{2}!", m_data.title, m_data.width, m_data.height);
            return -2;
        }

        // Ошибка инициализации рендера
        if (!Render_OpenGL::init(m_pWindow))
        {
            LOG_CRITICAL("Failed to initialize OpenGL renderer");
            return -3;
        }

        // Контекст окна и структура 
        glfwSetWindowUserPointer(m_pWindow, &m_data);

        // Обработка клавиш с клавиатуры
        glfwSetKeyCallback(m_pWindow, [](GLFWwindow* pWindow, int key, int scancode, int action, int mods) {
                // Окно
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                // Обработка событий (нажатие, зажатие и отпускание клавиш)
                switch (action) {
                case GLFW_PRESS: {
                    EventKeyPressed event(static_cast<KeyCode>(key), false);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE: {
                    EventKeyReleased event(static_cast<KeyCode>(key));
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_REPEAT: {
                    EventKeyPressed event(static_cast<KeyCode>(key), true);
                    data.eventCallbackFn(event);
                    break;
                }
                }
            });

        // Обработка мышки
        glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* pWindow, int button, int action, int mods){
                // Окно, координаты и их получение
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                double x_pos;
                double y_pos;
                glfwGetCursorPos(pWindow, &x_pos, &y_pos);
                // Обработка событий (нажатие и отпускание)
                switch (action){
                case GLFW_PRESS: {
                    EventMouseButtonPressed event(static_cast<MouseButton>(button), x_pos, y_pos);
                    data.eventCallbackFn(event);
                    break;
                }
                case GLFW_RELEASE: {
                    EventMouseButtonReleased event(static_cast<MouseButton>(button), x_pos, y_pos);
                    data.eventCallbackFn(event);
                    break;
                }
                }
            }
        );

        // Изменения окна 
        glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            data.width = width;
            data.height = height;

            EventWindowResize event(width, height);
            data.eventCallbackFn(event);
        });

        // Изменения положения курсора
        glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double x, double y) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventMouseMoved event(x, y);
            data.eventCallbackFn(event);
            });

        // Закрытие окна
        glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventWindowClose event;
            data.eventCallbackFn(event);
        });

        // Задаём границы отрисовки экрана для треугольника
        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height){
                Render_OpenGL::set_viewport(width, height);
            });

        // При создании окна
        UIModule::on_window_create(m_pWindow);

        return 0;
	}

    // Функция закрытия игрового движка
    void Window::shutdown() {
        // При закрытии окна
        UIModule::on_window_close();
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    // Функция обновления игрового движка
    void Window::on_update() {
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }

    // Функция для получения координат мышки
    glm::vec2 Window::get_current_cursor_position() const {
        double x_pos;
        double y_pos;
        glfwGetCursorPos(m_pWindow, &x_pos, &y_pos);
        return { x_pos, y_pos };
    }

}