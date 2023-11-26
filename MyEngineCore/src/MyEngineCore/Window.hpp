// Заголовочный файл с классом приложения для игрового движка
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
		using EventCallbackFn = std::function<void(Event&)>;

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
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};

		int init();
		void shutdown();

		GLFWwindow* m_pWindow = nullptr;
		WindowData m_data;
	};

}