#include "MyEngineCore/Application.hpp"
#include "MyEngineCore/Log.hpp"
#include "MyEngineCore/Window.hpp"
#include "MyEngineCore/Event.hpp"
#include "MyEngineCore/Input.hpp"

#include "MyEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "MyEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "MyEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "MyEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "MyEngineCore/Rendering/OpenGL/Texture_2D.hpp"
#include "MyEngineCore/Camera.hpp"
#include "MyEngineCore/Rendering/OpenGL/Render_OpenGL.hpp"
#include "MyEngineCore/Modules/UIModule.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

namespace MyEngine {

    // Отрисовка куба на экране

    // Массив с позициями
    GLfloat positions_coords[] = {
        // front
        -1.0f, -1.f, -1.f,   1.f, 0.f,
        -1.0f,  1.f, -1.f,   0.f, 0.f,
        -1.0f, -1.f,  1.f,   1.f, 1.f,
        -1.0f,  1.f,  1.f,   0.f, 1.f,

        // back
         1.0f, -1.f, -1.f,   1.f, 0.f,
         1.0f,  1.f, -1.f,   0.f, 0.f,
         1.0f, -1.f,  1.f,   1.f, 1.f,
         1.0f,  1.f,  1.f,   0.f, 1.f
    };

    // Массив для отрисовки граней 
    GLuint indices[] = {
        0, 1, 2, 3, 2, 1,
        0, 1, 2, 3, 2, 1, // front
        4, 5, 6, 7, 6, 5, // back
        0, 4, 6, 0, 2, 6, // right
        1, 5, 3, 3, 7, 5, // left
        3, 7, 2, 7, 6, 2, // top
        1, 5, 0, 5, 0, 4  // bottom
    };

    // Функция отрисовки текстуры
    void generate_circle(unsigned char* data, const unsigned int width, const unsigned int height, const unsigned int center_x,
        const unsigned int center_y, const unsigned int radius, const unsigned char color_r, const unsigned char color_g,
        const unsigned char color_b){
        for (unsigned int x = 0; x < width; ++x){
            for (unsigned int y = 0; y < height; ++y){
                if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius){
                    data[3 * (x + width * y) + 0] = color_r;
                    data[3 * (x + width * y) + 1] = color_g;
                    data[3 * (x + width * y) + 2] = color_b;
                }
            }
        }
    }

    // Генерация текстуры смайлика
    void generate_smile_texture(unsigned char* data, const unsigned int width, const unsigned int height){
        // background
        for (unsigned int x = 0; x < width; ++x){
            for (unsigned int y = 0; y < height; ++y){
                data[3 * (x + width * y) + 0] = 200;
                data[3 * (x + width * y) + 1] = 191;
                data[3 * (x + width * y) + 2] = 231;
            }
        }

        // face - лицо
        generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);

        // smile - улыбка/рот
        generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
        generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);

        // eyes - глаза
        generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
        generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);
    }

    // Генерация текстуры двух квадратов
    void generate_quads_texture(unsigned char* data, const unsigned int width, const unsigned int height) {
        for (unsigned int x = 0; x < width; ++x){
            for (unsigned int y = 0; y < height; ++y){
                if ((x < width / 2 && y < height / 2) || x >= width / 2 && y >= height / 2){
                    data[3 * (x + width * y) + 0] = 0;
                    data[3 * (x + width * y) + 1] = 0;
                    data[3 * (x + width * y) + 2] = 0;
                }
                else {
                    data[3 * (x + width * y) + 0] = 255;
                    data[3 * (x + width * y) + 1] = 255;
                    data[3 * (x + width * y) + 2] = 255;
                }
            }
        }
    }

    // Вертексный шейдер (трасформация координат и вычисления связанные с ними) 
    const char* vertex_shader =
        R"( // Версия шейдера
        #version 460
        // Входные данные шейдера (позиция и координаты текстуры)
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec2 texture_coord;

        // Матрица вида, матрица проекции и переменной кадра
        uniform mat4 model_matrix;
        uniform mat4 view_projection_matrix;
        uniform int current_frame;  

        // Выходные данные шейдера (векторы и координаты тексты смайлика и двух квадратов)
        out vec2 tex_coord_smile;
        out vec2 tex_coord_quads;

        // Функция для цвета
        void main() {

            // Задаём текстуры (смайлика и двух квадратов)
            tex_coord_smile = texture_coord;
            tex_coord_quads = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);

            // Задаём позицию вертекса (4 параметр - перспектива)
           gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0); 
        })";

    // Фрагментный шейдер
    const char* fragment_shader =
        R"(// Версия шейдера
        #version 460

        // Входные данные (координаты текстуры и их хранение)
        in vec2 tex_coord_smile;
        in vec2 tex_coord_quads;
        layout (binding = 0) uniform sampler2D InTexture_Smile;
        layout (binding = 1) uniform sampler2D InTexture_Quads;

        // Выходные данные (итоговый цвет)
        out vec4 frag_color;

        // Функция для задания цвета
        void main() {
           // Задаём цвет (4 параметр - прозрачность) и текстуру
           frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);
        })";

    // Указатель на шейдерную программа и обработчик
    std::unique_ptr<ShaderProgram> p_shader_program;
    // Указатель на двойной вертексный буфер, текстура смайлика и текстура двух квадратов и одиначный вертексный буфер
    std::unique_ptr<VertexBuffer> p_cube_positions_vbo;
    std::unique_ptr<IndexBuffer> p_cube_index_buffer;
    std::unique_ptr<Texture2D> p_texture_smile;
    std::unique_ptr<Texture2D> p_texture_quads;
    std::unique_ptr<VertexArray> p_vao;
    // Массивы для работы с матрицой смежности (сжатие/растяжение, вращение и перемещение) 
    float scale[3] = { 1.f, 1.f, 1.f };
    float rotate = 0.f;
    float translate[3] = { 0.f, 0.f, 0.f };
    // Создаём переменную для отрисовки заднего фона
    float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.f };

    // Добавляем ещё несколько кубов в сцену
    std::array<glm::vec3, 5> positions = {
            glm::vec3(-2.f, -2.f, -4.f),
            glm::vec3(-5.f,  0.f,  3.f),
            glm::vec3(2.f,  1.f, -2.f),
            glm::vec3(4.f, -3.f,  3.f),
            glm::vec3(1.f, -7.f,  1.f)
    };

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

        // Обработка события нажатия мышки
        m_event_dispatcher.add_event_listener<EventMouseButtonPressed>([&](EventMouseButtonPressed& event){
                LOG_INFO("[Mouse button pressed: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
                Input::PressMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, true);
            });

        // Обработка события нажатия мышки
        m_event_dispatcher.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& event){
                LOG_INFO("[Mouse button released: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
                Input::ReleaseMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, false);
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

        // Параметры для текстуры 
        const unsigned int width = 1000;
        const unsigned int height = 1000;
        const unsigned int channels = 3;
        auto* data = new unsigned char[width * height * channels];

        // Создание текстуры смайлика, инициализация и присваивание текущего параметра
        generate_smile_texture(data, width, height);
        p_texture_smile = std::make_unique<Texture2D>(data, width, height);
        p_texture_smile->bind(0);
        
        // Создание текстуры двух квадратов, инициализация и присваивание текущего параметра
        generate_quads_texture(data, width, height);
        p_texture_quads = std::make_unique<Texture2D>(data, width, height);
        p_texture_quads->bind(1);

        delete[] data;

        // Работа с шейдерной программой
        //---------------------------------------//
        // Инициализация программы шейдеров с выбранными шейдерами
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->is_compiled()) { return false; }

        // Два массива памяти для видеокарты

        // Буферы для позиции и цвета
        BufferLayout buffer_layout_1vec3{
            ShaderDataType::Float3
        };

        BufferLayout buffer_layout_vec3_vec2{
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };

        // Обработка значений

        // Добавляем буферы позиции и цвета в массив соответственно
        p_vao = std::make_unique<VertexArray>();
        p_cube_positions_vbo = std::make_unique<VertexBuffer>(positions_coords, sizeof(positions_coords), buffer_layout_vec3_vec2);
        p_cube_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        p_vao->add_vertex_buffer(*p_cube_positions_vbo);
        p_vao->set_index_buffer(*p_cube_index_buffer);

        //---------------------------------------//

        // Переменная кадров
        static int current_frame = 0;

        // Включаем тест глубины
        Render_OpenGL::enable_depth_test();

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
            p_shader_program->set_matrix4("model_matrix", model_matrix);
            //p_shader_program->set_int("current_frame", current_frame++);

            // Задаём камеру в пространстве
            camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
            p_shader_program->set_matrix4("view_projection_matrix", camera.get_projection_matrix()* camera.get_view_matrix());

            // Отрисовка рнедера
            Render_OpenGL::draw(*p_vao);

            // Отрисовка кубов в сцене
            for (const glm::vec3& current_position : positions){
                glm::mat4 translate_matrix(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    current_position[0], current_position[1], current_position[2], 1);
                p_shader_program->set_matrix4("model_matrix", translate_matrix);
                Render_OpenGL::draw(*p_vao);
            }

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

    // Получения позиции курсора
    glm::vec2 Application::get_current_cursor_position() const {
        return m_pWindow->get_current_cursor_position();
    }

}