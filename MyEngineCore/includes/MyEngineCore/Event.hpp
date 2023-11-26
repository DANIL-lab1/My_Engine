// Заголовочный файл для событий

#pragma once

#include <functional>
#include <array>

namespace MyEngine {
	
	enum class EventType {

		// Расширение окна
		WindowResize = 0,
		// Закрытие окна
		WindowClose,

		// Нажатие клавиши
		KeyPressed,
		// Отпускание клавиши
		KeyReleased,

		// Нажатие мышки 
		MouseButtonPressed,
		// Отпускание мышки
		MouseButtonReleased,
		// Движение мышки
		MouseMoved,

		// Общее количество событий
		EventsCount
	};

	// Структура событий
	struct BaseEvent {
		virtual ~BaseEvent() = default;
		virtual EventType get_type() const = 0;
	};

	// Класс обработчика событий
	class EventDispatcher{
	public:
		template<typename EventType>
		// Добавления обработки
		void add_event_listener(std::function<void(EventType&)> callback){
			// Поиск названия события
			auto baseCallback = [func = std::move(callback)](BaseEvent& e){
					if (e.get_type() == EventType::type){
						func(static_cast<EventType&>(e));
					}
				};
			m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
		}

		// Поиск события
		void dispatch(BaseEvent& event){
			auto& callback = m_eventCallbacks[static_cast<size_t>(event.get_type())];
			if (callback){
				callback(event);
			}
		}

	private:
		// Массив с событиями, количество событий
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;
	};

	// Стурктура для события "движение мышки"
	struct EventMouseMoved : public BaseEvent {
		// Конструктор 
		EventMouseMoved(const double new_x, const double new_y): x(new_x), y(new_y) {}

		// Возвращение типа события
		virtual EventType get_type() const override {
			return type;
		}

		// Задание новых переменных
		double x;
		double y;

		// Тип события
		static const EventType type = EventType::MouseMoved;
	};

	// Стурктура для события "изменения окна"
	struct EventWindowResize : public BaseEvent {
		// Конструктор
		EventWindowResize(const unsigned int new_width, const unsigned int new_heigth) : width(new_width), height(new_heigth) {}

		// Возвращение типа события
		virtual EventType get_type() const override {
			return type;
		}

		// Задание новых переменных
		unsigned int width;
		unsigned int height;

		// Тип события
		static const EventType type = EventType::WindowResize;
	};

	// Стурктура для события "закрытие окна"
	struct EventWindowClose : public BaseEvent {

		// Возвращение типа события
		virtual EventType get_type() const override {
			return type;
		}

		// Тип события
		static const EventType type = EventType::WindowClose;
	};
}