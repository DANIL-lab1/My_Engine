// Файл для работы с окном игрового движка

#include "MyEngineCore/Window.hpp"
#include "MyEngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <imgui/backends/imgui_impl_glfw.h>

namespace MyEngine {
	
    // Переменная инициализация
    static bool s_GLFW_initialized = false;

    // Массив с позициями треугольника
    GLfloat points[] = {
       0.0f,  0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f
    };

    // Массив с цветом каждой вершины треугольника
    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    // Вертексный шейдер (трасформация координат и вычисления связанные с ними) 
    const char* vertex_shader =
        // Версия шейдера
        "#version 460\n"
        // Входные данные шейдера (позиция и цвет)
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        // Выходные данные шейдера (векторы)
        "out vec3 color;"
        // Функция для цвета
        "void main() {"
            // Задаём цвет вертекса
        "   color = vertex_color;"
            // Задаём позицию вертекса (4 параметр - перспектива)
        "   gl_Position = vec4(vertex_position, 1.0);"
        "}";

    // Фрагментный шейдер
    const char* fragment_shader =
        // Версия шейдера
        "#version 460\n"
        // Входные данные (цвет)
        "in vec3 color;"
        // Выходные данные (итоговый цвет)
        "out vec4 frag_color;"
        // Функция для задания цвета
        "void main() {"
            // Задаём цвет (4 параметр - прозрачность)
        "   frag_color = vec4(color, 1.0);"
        "}";

    // Две переменные для шейдеров (хранения и обработки)
    GLuint shader_program;
    GLuint vao;

    // Конструктор и запуск игрового движка
	Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_data({ std::move(title), width, height }) {
		int resultCode = init();

        // Проверка версии 
        IMGUI_CHECKVERSION();
        // Создание контекстка
        ImGui::CreateContext();
        // Инициализация openGL
        ImGui_ImplOpenGL3_Init();
        // Подключаем OpenGL для редактирования
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
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

        // Задаём границы отрисовки экрана для треугольника
        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                glViewport(0, 0, width, height);
            });

        // Вертексный шейдер (создание, задание и инициализация его в проекте)
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader, nullptr);
        glCompileShader(vs);

        // Фрагментный шейдер (создание, задание и инициализация его в проекте)
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment_shader, nullptr);
        glCompileShader(fs);

        // Обработка шейдеров с помощью шейдерной программы (инициализация программы, обработка двух шейдеров и создание итоговой программы) 
        shader_program = glCreateProgram();
        glAttachShader(shader_program, vs);
        glAttachShader(shader_program, fs);
        glLinkProgram(shader_program);

        // Удаляем использованные шейдеры
        glDeleteShader(vs);
        glDeleteShader(fs);

        // Два массива памяти для видеокарты

        // Создаём буфер для ингерации в него позиции шейдеров
        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        // Создаём буфер для ингерации в него цвета шейдеров
        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        // Обработка значений

        // Создаём вертекс для обработки наших значений шейдеров
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Обработка (связка) шейдера позиции
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Обработка (связка) шейдера цвета
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        return 0;
	}

    // Функция закрытия игрового движка
    void Window::shutdown() {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    // Функция обновления игрового движка
    void Window::on_update() {
        // Делаем выборку цвета фона динамичной
        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовка треугольника (подключение программы шейдеров, подключаем vertex array object (обработка), сама отрисовка треуголника)
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Задание размеров окна
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        // Создание фрейма для рисования
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Данные для окна
        ImGui::ShowDemoWindow();

        // Запуск работы приложения (инициализация, работа и закрытие)
        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background Color", m_background_color);
        ImGui::End();

        // Отрисовка данных и рендер
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }

}