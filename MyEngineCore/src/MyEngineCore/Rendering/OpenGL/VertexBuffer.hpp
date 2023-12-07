#pragma once

#include <vector>

namespace MyEngine {

    // Уникальный класс для опеределения типа перменных
    enum class ShaderDataType {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
    };

    // Структура для буферного элемента
    struct BufferElement {
        ShaderDataType type;
        uint32_t component_type;
        size_t components_count;
        size_t size;
        size_t offset;
        BufferElement(const ShaderDataType type);
    };

    // Класс для layout
    class BufferLayout {
    public:
        // Конструктор
        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_elements(std::move(elements)) {
            size_t offset = 0;
            m_stride = 0;
            for (auto& element : m_elements) {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

        const std::vector<BufferElement>& get_elements() const { return m_elements; }
        size_t get_stride() const { return m_stride; }

    private:
        std::vector<BufferElement> m_elements;
        size_t m_stride = 0;
    };


    class VertexBuffer {
    public:

        // Базовый универсальный класс
        enum class EUsage {
            Static,
            Dynamic,
            Stream
        };

        // Конструктор и деструктор
        VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout, const EUsage usage = VertexBuffer::EUsage::Static);
        ~VertexBuffer();

        // Удаляем пустой конструкторы и операторы присваиваний
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator = (const VertexBuffer&) = delete;
        VertexBuffer& operator = (VertexBuffer&& vertexBuffer) noexcept;
        VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

        // Установка вертексного буфера
        unsigned int get_handle() const { return m_id; }

        // Упаковка данных в layout
        const BufferLayout& get_layout() const { return m_buffer_layout; }

    private:
        // id вертексного буфера и упаковка данных (одинарные или двойные)
        unsigned int m_id = 0;
        BufferLayout m_buffer_layout;
    };

}