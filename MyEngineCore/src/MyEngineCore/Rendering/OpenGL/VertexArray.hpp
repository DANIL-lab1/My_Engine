#pragma once
#include "VertexBuffer.hpp"

namespace MyEngine {

    class VertexArray {
    public:
        //  онструктор и деструктор
        VertexArray();
        ~VertexArray();

        // ”дал€ем пустой конструкторы и операторы присваиваний
        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertex_buffer) noexcept;
        VertexArray(VertexArray&& vertex_buffer) noexcept;

        // ”становка вертексного массива (прив€зка буфера к массиву, установка и убирание)
        void add_buffer(const VertexBuffer& vertex_buffer);
        void bind() const;
        static void unbind();

    private:
        // id и количество элементов
        unsigned int m_id = 0;
        unsigned int m_elements_count = 0;
    };

}