#pragma once

namespace MyEngine {

    class VertexBuffer {
    public:

        // Базовый универсальный класс
        enum class EUsage{
            Static,
            Dynamic,
            Stream
        };

        // Конструктор и деструктор
        VertexBuffer(const void* data, const size_t size, const EUsage usage = VertexBuffer::EUsage::Static);
        ~VertexBuffer();

        // Удаляем пустой конструкторы и операторы присваиваний
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
        VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

        // Установка вертексного буфера (установка и убирание)
        void bind() const;
        static void unbind();

    private:
        // id вертексного буфера
        unsigned int m_id = 0;
    };

}