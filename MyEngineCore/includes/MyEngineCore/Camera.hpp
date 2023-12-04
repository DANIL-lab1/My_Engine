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
        const glm::mat4& get_view_matrix() const { return m_view_matrix; }
        const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }

        // Движение вперёд, вправо и вверх
        void move_forward(const float delta);
        void move_right(const float delta);
        void move_up(const float delta);

        // Получение позиции и вращения
        const glm::vec3& get_camera_position() const { return m_position; }
        const glm::vec3& get_camera_rotation() const { return m_rotation; }

        // Функция для перемещения и вращения одновременно
        // movement_delta.x - forward, movement_delta.y - right, movement_delta.z - up
        // rotation_delta.x - roll, rotation_delta.y - pitch, rotation_delta.z - yaw
        void add_movement_and_rotatition(const glm::vec3& movement_delta, const glm::vec3& rotation_delta);

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

        // Изначальные векторы в мире
        const glm::vec3 s_world_up{ 0.f, 0.f, 1.f };
        const glm::vec3 s_world_right{ 0.f, -1.f, 0.f };
        const glm::vec3 s_world_forward{ 1.f, 0.f, 0.f };

        // Матрица вида и проекции
        glm::mat4 m_view_matrix;
        glm::mat4 m_projection_matrix;
    };
}