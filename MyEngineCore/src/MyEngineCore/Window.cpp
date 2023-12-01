// Файл для работы с окном игрового движка

#include "MyEngineCore/Window.hpp"
#include "MyEngineCore/Log.hpp"
#include "MyEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "MyEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "MyEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "MyEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "MyEngineCore/Camera.hpp"

#include "MyEngineCore/Rendering/OpenGL/Render_OpenGL.hpp"
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <imgui/backends/imgui_impl_glfw.h>

#include <glm/glm/mat3x3.hpp>
#include <glm/glm/trigonometric.hpp>

namespace MyEngine {

    // Массив с позициями и цветом одновременно
    GLfloat positions_colors2[] = {
       -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f
    };

    // Массив для рисование квадрата 
    GLuint indices[] = { 0, 1, 2, 3, 2, 1 };

    // Вертексный шейдер (трасформация координат и вычисления связанные с ними) 
    const char* vertex_shader =
        R"( // Версия шейдера
        #version 460
        // Входные данные шейдера (позиция и цвет, матрица модели и матрица проекции)
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec3 vertex_color;
        uniform mat4 model_matrix;
        uniform mat4 view_projection_matrix; 
        // Выходные данные шейдера (векторы)
        out vec3 color;
        // Функция для цвета
        void main() {
            // Задаём цвет вертекса
            color = vertex_color;
            // Задаём позицию вертекса (4 параметр - перспектива)
           gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0); 
        })";

    // Фрагментный шейдер
    const char* fragment_shader =
        R"(// Версия шейдера
        #version 460
        // Входные данные (цвет)
        in vec3 color;
        // Выходные данные (итоговый цвет)
        out vec4 frag_color;
        // Функция для задания цвета
        void main() {
            // Задаём цвет (4 параметр - прозрачность)
           frag_color = vec4(color, 1.0);
        })";

    // Указатель на шейдерную программа и обработчик
    std::unique_ptr<ShaderProgram> p_shader_program;
    // Указатель на двойной вертексный буфер и одиначный вертексный буфер
    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer> p_index_buffer;
    std::unique_ptr<VertexArray> p_vao;
    // Массивы для работы с матрицой смежности (сжатие/растяжение, вращение и перемещение) 
    float scale[3] = { 1.f, 1.f, 1.f };
    float rotate = 0.f;
    float translate[3] = { 0.f, 0.f, 0.f };

    float camera_position[3] = { 0.f, 0.f, 1.f };
    float camera_rotation[3] = { 0.f, 0.f, 0.f };
    bool perspective_camera = false;
    Camera camera;

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

        // Ошибка вызова рендера
        glfwSetErrorCallback([](int error_code, const char* description) {
            LOG_CRITICAL("GLFW error: {0}", description);
            });

        if (!glfwInit())
        {
            LOG_CRITICAL("Can't initialize GLFW!");
            return -1;
        }

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

        // Инициализация программы шейдеров с выбранными шейдерами
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()){ return false; }

        // Два массива памяти для видеокарты

        // Буферы для позиции и цвета
        BufferLayout buffer_layout_1vec3 {
            ShaderDataType::Float3
        };

        BufferLayout buffer_layout_2vec3 {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };

        // Обработка значений

        // Добавляем буферы позиции и цвета в массив соответственно
        p_vao = std::make_unique<VertexArray>();
        p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors2, sizeof(positions_colors2), buffer_layout_2vec3);
        p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        p_vao->add_vertex_buffer(*p_positions_colors_vbo);
        p_vao->set_index_buffer(*p_index_buffer);

        return 0;
	}

    // Функция закрытия игрового движка
    void Window::shutdown() {
        if (ImGui::GetCurrentContext()) {
            ImGui::DestroyContext();
        }
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    // Функция обновления игрового движка
    void Window::on_update() {
        // Установка цвета рендера и очистка
        Render_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        Render_OpenGL::clear();

        // Задание размеров окна
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        // Создание фрейма для рисования
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Данные для окна
        //ImGui::ShowDemoWindow();

        // Запуск работы приложения (инициализация, работа и закрытие)
        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background Color", m_background_color);
        
        // Задаём растяжение/сжатие, вращение и перемещение для объекта
        ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("translate", translate, -1.f, 1.f);

        // Задаём позицию, вращение и перспективу для камеры
        ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
        ImGui::Checkbox("Perspective camera", &perspective_camera);
        
        // Подключаем с помощью двойного массива
        p_shader_program->bind();

        // Матрица скарирования
        glm::mat4 scale_matrix(scale[0], 0, 0, 0,
            0, scale[1], 0, 0,
            0, 0, scale[2], 0,
            0, 0, 0, 1);

        // Матрица вращения
        float rotate_in_radians = glm::radians(rotate);
        glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
            -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        // Матрица перемещение
        glm::mat4 translate_matrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translate[0], translate[1], translate[2], 1);

        glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
        p_shader_program->setMatrix4("model_matrix", model_matrix);

        // Задаём камеру в пространстве
        camera.set_position_rotation(glm::vec3(camera_position[0], camera_position[1], camera_position[2]),
            glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
        camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
        p_shader_program->setMatrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());

        // Отрисовка рнедера
        Render_OpenGL::draw(*p_vao);
        ImGui::End();
        // Отрисовка данных и рендер
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }

}