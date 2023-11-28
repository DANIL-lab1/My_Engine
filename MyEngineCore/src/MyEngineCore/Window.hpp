// Заголовочный файл с классом окна игрового движка
#pragma once

#include <MyEngineCore/Event.hpp>
#include "MyEngineCore/Event.hpp"

#include <functional>
#include <string>

struct GLFWwindow;

namespace MyEngine {

	// Класс окна игрового движка
	class Window {
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		// Конструктор и деструктор
		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		// Операторы, которые в будушем нам пригодятся
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator = (const Window&) = delete;
		Window& operator = (Window&&) = delete;

		// Функция, которая вызывается каждый раз для обновления событий в окне
		void on_update();
		unsigned int get_width() const { return m_data.width; };
		unsigned int get_height() const { return m_data.height; };

		void set_event_callback(const EventCallbackFn& callback) {
			m_data.eventCallbackFn = callback;
		}

	private:
		// Структура окна с переменными: название, ширина, длина и обработчик событий
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};

		// Инициализация и закрытие окна
		int init();
		void shutdown();

		// Указатель на окно и имя структуры
		GLFWwindow* m_pWindow = nullptr;
		WindowData m_data;
		// Создаём переменную для отрисовки заднего фона
		float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.f };
	};

}