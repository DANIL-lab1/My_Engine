// Заголовочный файл с классом приложения для игрового движка
#pragma once

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

	private:
		std::unique_ptr<class Window> m_pWindow;
	};

}