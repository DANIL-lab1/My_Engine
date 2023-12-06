#include "UIModule.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

namespace MyEngine {

    // При создании окна
    void UIModule::on_window_create(GLFWwindow* pWindow)
    {
        // Проверка версии и создание контекста
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Создание и проверка докинга и проекции
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;

        // Инициализация и подключения для редактирования
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    }

    // При закрытии окна
    void UIModule::on_window_close()
    {
        // Деинициализация и закрытия всех окон
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // При начале отрисовки
    void UIModule::on_ui_draw_begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    // При конце отрисовки
    void UIModule::on_ui_draw_end()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}