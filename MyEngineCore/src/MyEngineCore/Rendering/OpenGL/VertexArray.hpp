#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace MyEngine {

    class VertexArray {
    public:
        // Конструктор и деструктор
        VertexArray();
        ~VertexArray();

        // Удаляем пустой конструкторы и операторы присваиваний
        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertex_buffer) noexcept;
        VertexArray(VertexArray&& vertex_buffer) noexcept;

        // Установка вертексного и индексного буфера, установка и снимание текущего буфера и получение текущего количества элементов
        void add_vertex_buffer(const VertexBuffer& vertex_buffer);
        void set_index_buffer(const IndexBuffer& index_buffer);
        void bind() const;
        static void unbind();
        size_t get_indices_count() const { return m_indices_count; }

    private:
        // id и количество элементов
        unsigned int m_id = 0;
        unsigned int m_elements_count = 0;
        size_t m_indices_count = 0;
    };
}