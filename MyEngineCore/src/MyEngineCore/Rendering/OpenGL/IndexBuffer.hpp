#pragma once
#include "VertexBuffer.hpp"

namespace MyEngine {

    class IndexBuffer {
    public:

        //  онструктор и деструктор
        IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage = VertexBuffer::EUsage::Static);
        ~IndexBuffer();

        // ƒополнительные конструкторы и операторы присваивани€
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer& operator=(IndexBuffer&& index_buffer) noexcept;
        IndexBuffer(IndexBuffer&& index_buffer) noexcept;

        // ”становка текущего, убираение текущего и количество элементов
        void bind() const;
        static void unbind();
        size_t get_count() const { return m_count; }

    private:
        // id и количество элементов
        unsigned int m_id = 0;
        size_t m_count;
    };

}