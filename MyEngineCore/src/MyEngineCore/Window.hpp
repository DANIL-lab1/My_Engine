// Заголовочный файл с классом приложения для игрового движка
#pragma once

#include <string>

struct GLFWwindow;

namespace MyEngine {

	// Класс окна игрового движка
	class Window {
	public:
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
		unsigned int get_width() const { return m_width; };
		unsigned int get_height() const { return m_height; };

	private:
		int init();
		void shutdown();

		GLFWwindow* m_pWindow;
		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
	};

}