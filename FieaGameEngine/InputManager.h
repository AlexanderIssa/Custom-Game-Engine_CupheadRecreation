#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

namespace Fiea::GameEngine
{
	class Window;

	class InputManager
	{
	public:
		static void SetupKeyInputs(Window& window);
		static bool GetKeyDown(int key);
		static bool GetKeyUp(int key);
		static bool GetMouseButtonDown(int btn);
		static bool GetMouseButtonUp(int btn);

		inline static struct Controller
		{
			int contollerConnected;
			const char* controllerName;
			int axesCount;
			const float* axes;
			int cButtonCount;
			const unsigned char* cButtons;
			GLFWgamepadstate state;
		} _controller;
	private:
		static void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);

		static std::unordered_map<int, bool> keys;
		static std::unordered_map<int, bool> buttons;
		

		//int InputManager::Controller::contollerConnected = 0;
		//const char* InputManager::Controller::controllerName = "DefaultController";
	};
}