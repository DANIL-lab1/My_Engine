#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace MyEngine {

    class Camera {
    public:
        // Выбор способа проекции камеры
        enum class ProjectionMode {
            Perspective,
            Orthographic
        };

        // Конструктор (позиция, поворот и способ проекцирования камеры)
        Camera(const glm::vec3& position = { 0, 0, 0 }, const glm::vec3& rotation = { 0, 0, 0 }, 
            const ProjectionMode projection_mode = ProjectionMode::Perspective);

        // Установка позиции, поворота и проекции камеры, получение матрицы вида и проекции
        void set_position(const glm::vec3& position);
        void set_rotation(const glm::vec3& rotation);
        void set_position_rotation(const glm::vec3& position, const glm::vec3& rotation);
        void set_projection_mode(const ProjectionMode projection_mode);
        // Установка камеры
        void set_far_clip_plane(const float far);
        void set_near_clip_plane(const float near);
        void set_viewport_size(const float width, const float height);
        void set_field_of_view(const float fov);
        const glm::mat4& get_view_matrix();
        const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }
        // Получение значений камеры (ближайшее изображение, дальнее и вид)
        const float get_far_clip_plane() const { return m_far_clip_plane; }
        const float get_near_clip_plane() const { return m_near_clip_plane; }
        const float get_field_of_view() const { return m_field_of_view; }

        // Движение вперёд, вправо и вверх
        void move_forward(const float delta);
        void move_right(const float delta);
        void move_up(const float delta);

        // Получение позиции и вращения
        const glm::vec3& get_position() const { return m_position; }
        const glm::vec3& get_rotation() const { return m_rotation; }

        // Функция для перемещения и вращения одновременно
        // movement_delta.x - forward, movement_delta.y - right, movement_delta.z - up
        // rotation_delta.x - roll, rotation_delta.y - pitch, rotation_delta.z - yaw
        void add_movement_and_rotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta);

    private:

        void update_view_matrix();
        void update_projection_matrix();

        // Позиция, поворот и проекция камеры, матрицы вида и проекции
        glm::vec3 m_position;
        glm::vec3 m_rotation; // X - Roll, Y - Pitch, Z - Yaw
        ProjectionMode m_projection_mode;

        // Векторы направлений
        glm::vec3 m_direction;
        glm::vec3 m_right;
        glm::vec3 m_up;

        // Переменные камеры (ближайшее изображение, дальнее изображение, вид (ширина и высота) и угол вида)
        float m_far_clip_plane{ 100.f };
        float m_near_clip_plane{ 0.1f };
        float m_viewport_width{ 800.f };
        float m_viewport_height{ 600.f };
        float m_field_of_view{ 60.f };

        // Изначальные векторы в мире
        const glm::vec3 s_world_up{ 0.f, 0.f, 1.f };
        const glm::vec3 s_world_right{ 0.f, -1.f, 0.f };
        const glm::vec3 s_world_forward{ 1.f, 0.f, 0.f };

        // Матрица вида и проекции, проверка обновления
        glm::mat4 m_view_matrix;
        glm::mat4 m_projection_matrix;
        bool m_update_view_matrix = false;
    };
}