#include "pch.h"
#include "Window.h"
#include <iostream>

namespace Fiea::GameEngine
{
	void Window::createWindow(const std::string& title, int width, int height)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		if (!getWindow()) {
			glfwTerminate();
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::logic_error("Failed to initialize GLAD");
		}

		glViewport(0, 0, width, height);
	}

	bool Window::shouldClose()
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		return glfwWindowShouldClose(window);
	}
}