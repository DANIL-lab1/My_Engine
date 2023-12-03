// Главный файл всей программы

#include <iostream>
#include <memory>
#include "MyEngineCore/Application.hpp"
#include <imgui/imgui.h>

using namespace std;

class MyEngineEditor :public MyEngine::Application {
	virtual void on_update() override {
		//cout << "Update frame: " << frame++ << endl;
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