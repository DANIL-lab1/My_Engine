#pragma once

#include "MyEngineCore/Application.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

namespace MyEngine {

    // Реализация конструктора
	Application::Application() {}
	
    // Реализация деструктора
	Application::~Application() {}

    // Функция запуска приложения. В данный момент мы передали ей код для создания базового окна
	int Application::start(unsigned int window_width, unsigned int window_height, const char* title) {
        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            // Реализация функции обновления
            on_update();
        }

        glfwTerminate();
        return 0;
	}

}