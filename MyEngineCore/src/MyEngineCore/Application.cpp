#include "MyEngineCore/Application.hpp"
#include "MyEngineCore/Log.hpp"
#include "MyEngineCore/Window.hpp"

#include <iostream>

namespace MyEngine {

    // Реализация конструктора
	Application::Application() {
        LOG_INFO("Starting Application!");
    }
	
    // Реализация деструктора
	Application::~Application() {
        LOG_INFO("Closing Application!");
    }

    // Функция запуска приложения. В данный момент мы передали ей код для создания базового окна
	int Application::start(unsigned int window_width, unsigned int window_height, const char* title) {
        // Окно с параметрами: название, ширина и высота
        m_pWindow = std::make_unique<Window>(title, window_width, window_height);
        
        // Обработка события движения мышки
        m_event_dispatcher.add_event_listener<EventMouseMoved>(
            [](EventMouseMoved& event) {
                //LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
            });

        // Обработка события изменения расширения экрана пользователем
        m_event_dispatcher.add_event_listener<EventWindowResize>(
            [](EventWindowResize& event) {
                LOG_INFO("[Resized] Change size to {0}x{1}", event.height, event.width);
            });

        // Обработка события закрытия окна 
        m_event_dispatcher.add_event_listener<EventWindowClose>(
            [&](EventWindowClose& event) {
                LOG_INFO("[Window Closed]");
                m_bCloseWindow = true;
            });

        // Вызов обработчиков функции
        m_pWindow->set_event_callback(
            [&](BaseEvent& event){
                m_event_dispatcher.dispatch(event);
            });

        // Пока окно не закрыто, обновляем
        while (!m_bCloseWindow) {
            m_pWindow->on_update();
            on_update();
        }
        m_pWindow = nullptr;

        return 0;
	}

}