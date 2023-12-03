// Заголовочный файл с классом приложения для игрового движка
#pragma once

#include "MyEngineCore/Event.hpp"
#include "MyEngineCore/Camera.hpp"

#include <memory>

namespace MyEngine {
	
	// Класс приложения
	class Application {
	public:
		// Конструктор и деструктор
		Application();
		virtual ~Application();

		// Операторы, которые в будушем нам пригодятся
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator = (const Application&) = delete;
		Application& operator = (Application&&) = delete;

		// Переменная, в которой задаются настройки окна(ширина и высота) и название
		virtual int start(unsigned int window_width, unsigned int window_height, const char* title);

		// Функция, которая вызывается каждый раз для обновления событий в окне
		virtual void on_update() {}

		// Функция для отрисовки кастомного интерфейса 
		virtual void on_ui_draw() {}

		// Параметры для камеры и камера сама по себе
		float camera_position[3] = { 0.f, 0.f, 1.f };
		float camera_rotation[3] = { 0.f, 0.f, 0.f };
		bool perspective_camera = true;
		Camera camera;

	private:
		// Переменная для работы с окном
		std::unique_ptr<class Window> m_pWindow;

		// Обработчик событий и переменная состояния закрытия окна
		EventDispatcher m_event_dispatcher;
		bool m_bCloseWindow = false;
	};

}