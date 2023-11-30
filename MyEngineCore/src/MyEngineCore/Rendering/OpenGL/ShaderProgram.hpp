#pragma once

#include <glm/glm/mat4x4.hpp>
namespace MyEngine {

    class ShaderProgram
    {
    public:
        // —оздаЄм конструктор, копирующий конструктор, оператор присваивани€ и деструктор
        ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
        ShaderProgram(ShaderProgram&&);
        ShaderProgram& operator=(ShaderProgram&&);
        ~ShaderProgram();

        // ”дал€ем пустой конструктор, копирующий конструктор и оператор присваивани€
        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        // ”становка шейдера (установка шейдера и убирание шейдера, установка матрицы)
        void bind() const;
        static void unbind();
        bool isCompiled() const { return m_isCompiled; }
        void setMatrix4(const char* name, const glm::mat4& matrix) const;

    private:
        // ѕеременные (удачна€ компил€ци€ и id шейдера)
        bool m_isCompiled = false;
        unsigned int m_id = 0;
    };

}