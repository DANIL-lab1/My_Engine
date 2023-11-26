// Файл для работы с окном игрового движка

#include "MyEngineCore/Window.hpp"
#include "MyEngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace MyEngine {
	
    // Переменная инициализация
    static bool s_GLFW_initialized = false;

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

        // Ошибка инициализации
        if (!s_GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        // Ошибка чтения данных
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Can't create window {0} width size {1}x{2}!", m_data.title, m_data.width, m_data.height);
            glfwTerminate();
            return -2;
        }

        // Контекст окна
        glfwMakeContextCurrent(m_pWindow);

        // Ошибка библиотеки GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -3;
        }

        // Контекст окна и структура 
        glfwSetWindowUserPointer(m_pWindow, &m_data);

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

        return 0;
	}

    // Функция закрытия игрового движка
    void Window::shutdown() {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    // Фукнция обновления игрового движка
    void Window::on_update() {
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }

}