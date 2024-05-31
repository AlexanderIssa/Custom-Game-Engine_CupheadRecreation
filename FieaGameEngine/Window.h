#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Fiea::GameEngine
{
	class Window
	{
	public:
		Window() = default;
		~Window() = default;

		void createWindow(const std::string& title, int width, int height);

		GLFWwindow* getWindow() const { return window; }

		bool shouldClose();

	private:
		GLFWwindow* window;
		int height;
		int width;
		std::string title;
	};
}