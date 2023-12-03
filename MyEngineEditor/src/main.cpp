// Главный файл всей программы

#include <iostream>
#include <memory>

#include <MyEngineCore/Input.hpp>
#include "MyEngineCore/Application.hpp"
#include <imgui/imgui.h>

using namespace std;

class MyEngineEditor :public MyEngine::Application {
	virtual void on_update() override {
		//cout << "Update frame: " << frame++ << endl;
        // Передвижение камеры (влево, вверх, вправо и вниз)
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_W)){
            camera_position[2] -= 0.01f;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_S)){
            camera_position[2] += 0.01f;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_A)){
            camera_position[0] -= 0.01f;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_D)){
            camera_position[0] += 0.01f;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_E)){
            camera_position[1] += 0.01f;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_Q)){
            camera_position[1] -= 0.01f;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_UP)){
            camera_rotation[0] += 0.5f;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_DOWN)){
            camera_rotation[0] -= 0.5f;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_RIGHT)){
            camera_rotation[1] -= 0.5f;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_LEFT)){
            camera_rotation[1] += 0.5f;
        }
	}

	// Начальная отрисовка
	virtual void on_ui_draw() override
	{
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
		ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
		ImGui::Checkbox("Perspective camera", &perspective_camera);
		ImGui::End();
	}

	int frame = 0;
};

int main() {
	
	// Создание приложения
	auto p_MyEngineEditor = make_unique<MyEngineEditor>();

	// Изначальные настройки приложения
	int returnCode = p_MyEngineEditor ->start(1024, 768, "My Engine Editor");

	//cin.get();

	return returnCode;
}