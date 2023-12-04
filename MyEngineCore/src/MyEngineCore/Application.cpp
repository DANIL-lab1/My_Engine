#include "MyEngineCore/Application.hpp"
#include "MyEngineCore/Log.hpp"
#include "MyEngineCore/Window.hpp"
#include "MyEngineCore/Event.hpp"
#include "MyEngineCore/Input.hpp"

#include "MyEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "MyEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "MyEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "MyEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "MyEngineCore/Camera.hpp"
#include "MyEngineCore/Rendering/OpenGL/Render_OpenGL.hpp"
#include "MyEngineCore/Modules/UIModule.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

namespace MyEngine {

    // Массив с позициями и цветом одновременно
    GLfloat positions_colors2[] = {
        0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
        0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
        0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
        0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f
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
    // Создаём переменную для отрисовки заднего фона
    float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.f };

    // Реализация конструктора
	Application::Application() {
        LOG_INFO("Starting Application!");
    }
	
    // Реализация деструктора
	Application::~Application() {
        LOG_INFO("Closing Application!");
    }

    // Функция запуска приложения. В данный момент мы передали ей код для создания базового окна
	int Application::start(unsigned int window_width, unsigned int window_height, const char* title) {
        // Окно с параметрами: название, ширина и высота
        m_pWindow = std::make_unique<Window>(title, window_width, window_height);
        
        // Обработка события движения мышки
        m_event_dispatcher.add_event_listener<EventMouseMoved>(
            [](EventMouseMoved& event) {
                //LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
            });

        // Обработка события изменения расширения экрана пользователем
        m_event_dispatcher.add_event_listener<EventWindowResize>(
            [](EventWindowResize& event) {
                LOG_INFO("[Resized] Change size to {0}x{1}", event.height, event.width);
            });

        // Обработка события закрытия окна 
        m_event_dispatcher.add_event_listener<EventWindowClose>(
            [&](EventWindowClose& event) {
                LOG_INFO("[Window Closed]");
                m_bCloseWindow = true;
            });

        // Обработка события нажатия клавиши
        m_event_dispatcher.add_event_listener<EventKeyPressed>([&](EventKeyPressed& event) {
            // Тип char ограничен, поэтому обрабатываем только до символа "z"
            if (event.key_code <= KeyCode::KEY_Z) {
                // Проверка, зажата ли клавиша или только нажата
                if (event.repeated) {
                    LOG_INFO("[Key pressed: {0}, repeated", static_cast<char>(event.key_code));
                }
                else {
                    LOG_INFO("[Key pressed: {0}", static_cast<char>(event.key_code));
                }
            }
            Input::PressKey(event.key_code);
        });

        // Обработка событий отпускания клавиши
        m_event_dispatcher.add_event_listener<EventKeyReleased>([&](EventKeyReleased& event) {
            // Тип char ограничен, поэтому обрабатываем только до символа "z"
            if (event.key_code <= KeyCode::KEY_Z){
                LOG_INFO("[Key released: {0}", static_cast<char>(event.key_code));
            }
            Input::ReleaseKey(event.key_code);
        });

        // Вызов обработчиков функции
        m_pWindow->set_event_callback(
            [&](BaseEvent& event){
                m_event_dispatcher.dispatch(event);
            });



        // Работа с шейдерной программой
        //---------------------------------------//
        // Инициализация программы шейдеров с выбранными шейдерами
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) { return false; }

        // Два массива памяти для видеокарты

        // Буферы для позиции и цвета
        BufferLayout buffer_layout_1vec3{
            ShaderDataType::Float3
        };

        BufferLayout buffer_layout_2vec3{
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
        //---------------------------------------//

        // Пока окно не закрыто, обновляем
        while (!m_bCloseWindow) {

            // Установка цвета рендера и очистка
            Render_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
            Render_OpenGL::clear();

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
            camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
            p_shader_program->setMatrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());

            // Отрисовка рнедера
            Render_OpenGL::draw(*p_vao);


            // Отрисовка интерфейса
            //---------------------------------------//
            // При отрисовки окна
            UIModule::on_ui_draw_begin();
            bool show = true;
            UIModule::ShowExampleAppDockSpace(&show);

            // Данные для окна
            ImGui::ShowDemoWindow();

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

            ImGui::End();
            //---------------------------------------//
            
            on_ui_draw();
            // При конце отрисовки
            UIModule::on_ui_draw_end();

            m_pWindow->on_update();
            on_update();
        }
        m_pWindow = nullptr;
        //---------------------------------------//

        return 0;
	}

}