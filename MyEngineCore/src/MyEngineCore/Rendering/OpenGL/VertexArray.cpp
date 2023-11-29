#include "VertexArray.hpp"

#include "MyEngineCore/Log.hpp"

#include <glad/glad.h>

namespace MyEngine {

    // Конструктор
    VertexArray::VertexArray(){
        glGenVertexArrays(1, &m_id);
    }

    // Деструктор 
    VertexArray::~VertexArray(){
        glDeleteVertexArrays(1, &m_id);
    }

    // Перемещающий оператор
    VertexArray& VertexArray::operator=(VertexArray&& vertex_array) noexcept {
        m_id = vertex_array.m_id;
        m_elements_count = vertex_array.m_id;
        vertex_array.m_id = 0;
        vertex_array.m_elements_count = 0;
        return *this;
    }

    // Конструктор
    VertexArray::VertexArray(VertexArray&& vertex_array) noexcept
        : m_id(vertex_array.m_id), m_elements_count(vertex_array.m_elements_count){
        vertex_array.m_id = 0;
        vertex_array.m_elements_count = 0;
    }

    // Задание текущего вертексного массива
    void VertexArray::bind() const {
        glBindVertexArray(m_id);
    }

    // Снимание текущего вертексного массива
    void VertexArray::unbind(){
        glBindVertexArray(0);
    }

    // Добавление буфера к массиву
    void VertexArray::add_buffer(const VertexBuffer& vertex_buffer){
        bind();
        vertex_buffer.bind();

        // Расчёт параметров для буфера
        for (const BufferElement& current_element : vertex_buffer.get_layout().get_elements()) {
            glEnableVertexAttribArray(m_elements_count);
            glVertexAttribPointer(
                m_elements_count,
                static_cast<GLint>(current_element.components_count),
                current_element.component_type,
                GL_FALSE,
                static_cast<GLsizei>(vertex_buffer.get_layout().get_stride()),
                reinterpret_cast<const void*>(current_element.offset));
            ++m_elements_count;
        }
    }
}