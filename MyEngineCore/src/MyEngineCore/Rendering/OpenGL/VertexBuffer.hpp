#pragma once

namespace MyEngine {

    class VertexBuffer {
    public:

        // Ѕазовый универсальный класс
        enum class EUsage{
            Static,
            Dynamic,
            Stream
        };

        //  онструктор и деструктор
        VertexBuffer(const void* data, const size_t size, const EUsage usage = VertexBuffer::EUsage::Static);
        ~VertexBuffer();

        // ”дал€ем пустой конструкторы и операторы присваиваний
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
        VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

        // ”становка шейдера (установка шейдера и убирание шейдера)
        void bind() const;
        static void unbind();

    private:
        // id шейдера
        unsigned int m_id = 0;
    };

}