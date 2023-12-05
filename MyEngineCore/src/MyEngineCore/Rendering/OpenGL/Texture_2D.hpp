#pragma once

namespace MyEngine {

    class Texture2D {
    public:
        //  онструктор и деструктор
        Texture2D(const unsigned char* data, const unsigned int width, const unsigned int height);
        ~Texture2D();

        // ”дал€ем копирующий конструктор, оператор присваивани€, перемещающий оператор и перемещающий конструктор
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D& operator=(Texture2D&& texture) noexcept;
        Texture2D(Texture2D&& texture) noexcept;

        // «адаЄм текущие текстуры
        void bind(const unsigned int unit) const;

    private:
        // id, длина и ширина
        unsigned int m_id = 0;
        unsigned int m_width = 0;
        unsigned int m_height = 0;
    };

}