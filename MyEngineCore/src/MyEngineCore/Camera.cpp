#include "MyEngineCore/Camera.hpp"

#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace MyEngine {
    // Конструктор камеры
    Camera::Camera(const glm::vec3& position,
        const glm::vec3& rotation,
        const ProjectionMode projection_mode): m_position(position), m_rotation(rotation), m_projection_mode(projection_mode){
        // Обнвление матриц вида и проекции
        update_view_matrix();
        update_projection_matrix();
    }

    // Обновление матрицы вида
    void Camera::update_view_matrix() {

        // X - Roll, Y - Pitch, Z - Yaw
        const float roll_in_radians = glm::radians(m_rotation.x);
        const float pitch_in_radians = glm::radians(m_rotation.y);
        const float yaw_in_radians = glm::radians(m_rotation.z);

        // Матрицы для каждой оси

        // Матрица x
        const glm::mat3 rotate_matrix_x(1, 0, 0,
            0, cos(roll_in_radians), sin(roll_in_radians),
            0, -sin(roll_in_radians), cos(roll_in_radians));

        // Матрица y
        const glm::mat3 rotate_matrix_y(cos(pitch_in_radians), 0, -sin(pitch_in_radians),
            0, 1, 0,
            sin(pitch_in_radians), 0, cos(pitch_in_radians));

        // Матрица z
        const glm::mat3 rotate_matrix_z(cos(yaw_in_radians), sin(yaw_in_radians), 0,
            -sin(yaw_in_radians), cos(yaw_in_radians), 0,
            0, 0, 1);
    
        // Матрица Эйлера и преобразование с помощью этой матрицы
        const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;
        m_direction = glm::normalize(euler_rotate_matrix * s_world_forward);
        m_right = glm::normalize(euler_rotate_matrix * s_world_right);
        m_up = glm::cross(m_right, m_direction);

        // Матрица вида
        m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);
    }

    // Обновления матрицы проекции
    void Camera::update_projection_matrix() {
        // Проверка на то, какую камеру мы используем
        if (m_projection_mode == ProjectionMode::Perspective) {
            // Оптические настройки
            float r = 0.1f;
            float t = 0.1f;
            float f = 10;
            float n = 0.1f;
            // Матрица проекции
            m_projection_matrix = glm::mat4(n / r, 0, 0, 0,
                0, n / t, 0, 0,
                0, 0, (-f - n) / (f - n), -1,
                0, 0, -2 * f * n / (f - n), 0);
        }
        else {
            // Оптические настройки
            float r = 2;
            float t = 2;
            float f = 100;
            float n = 0.1f;
            // Матрица проекции
            m_projection_matrix = glm::mat4(1 / r, 0, 0, 0,
                0, 1 / t, 0, 0,
                0, 0, -2 / (f - n), 0,
                0, 0, (-f - n) / (f - n), 1);
        }
    }

    // Установка позиции камеры
    void Camera::set_position(const glm::vec3& position) {
        m_position = position;
        update_view_matrix();
    }

    // Установка поворота камеры
    void Camera::set_rotation(const glm::vec3& rotation) {
        m_rotation = rotation;
        update_view_matrix();
    }

    // Установка позиции и поворота камеры
    void Camera::set_position_rotation(const glm::vec3& position, const glm::vec3& rotation) {
        m_position = position;
        m_rotation = rotation;
        update_view_matrix();
    }

    // Установка проекции камеры
    void Camera::set_projection_mode(const ProjectionMode projection_mode) {
        m_projection_mode = projection_mode;
        update_projection_matrix();
    }

    // Движение вперёд
    void Camera::move_forward(const float delta) {
        m_position += m_direction * delta;
        update_view_matrix();
    }

    // Движение вправо
    void Camera::move_right(const float delta) {
        m_position += m_right * delta;
        update_view_matrix();
    }

    // Движение вверх
    void Camera::move_up(const float delta) {
        m_position += m_up * delta;
        update_view_matrix();
    }

    // Перемещение и вращение одновременно
    void Camera::add_movement_and_rotatition(const glm::vec3& movement_delta, const glm::vec3& rotation_delta)
    {
        m_position += m_direction * movement_delta.x;
        m_position += m_right * movement_delta.y;
        m_position += m_up * movement_delta.z;
        m_rotation += rotation_delta;
        update_view_matrix();
    }
}