// Главный файл всей программы

#include <iostream>
#include <memory>

#include <MyEngineCore/Input.hpp>
#include "MyEngineCore/Application.hpp"
#include <imgui/imgui.h>

using namespace std;

class MyEngineEditor :public MyEngine::Application {
	virtual void on_update() override {
        // Смещена ли камера и векторы перемещения
        bool move_camera = false;
        glm::vec3 movement_delta{ 0, 0, 0 };
        glm::vec3 rotation_delta{ 0, 0, 0 };
		//cout << "Update frame: " << frame++ << endl;
        // Передвижение камеры (влево, вверх, вправо и вниз)
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_W)){
            movement_delta.x += 0.05f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_S)){
            movement_delta.x -= 0.05f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_A)){
            movement_delta.y -= 0.05f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_D)){
            movement_delta.y += 0.05f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_E)){
            movement_delta.z += 0.05f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_Q)){
            movement_delta.z -= 0.05f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_UP)){
            rotation_delta.y -= 0.5f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_DOWN)){
            rotation_delta.y += 0.5f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_RIGHT)){
            rotation_delta.z -= 0.5f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_LEFT)){
            rotation_delta.z += 0.5f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_P)){
            rotation_delta.x += 0.5f;
            move_camera = true;
        }
        if (MyEngine::Input::IsKeyPressed(MyEngine::KeyCode::KEY_O)){
            rotation_delta.x -= 0.5f;
            move_camera = true;
        }
        if (move_camera){
            camera.add_movement_and_rotatition(movement_delta, rotation_delta);
        }
	}

	// Начальная отрисовка
	virtual void on_ui_draw() override {
        camera_position[0] = camera.get_camera_position().x;
        camera_position[1] = camera.get_camera_position().y;
        camera_position[2] = camera.get_camera_position().z;
        camera_rotation[0] = camera.get_camera_rotation().x;
        camera_rotation[1] = camera.get_camera_rotation().y;
        camera_rotation[2] = camera.get_camera_rotation().z;

		ImGui::Begin("Editor");
        if (ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f)){
            camera.set_position(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
        }
        if (ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f)){
            camera.set_rotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
        }
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