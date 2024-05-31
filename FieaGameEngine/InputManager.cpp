#include "pch.h"
#include "InputManager.h"
#include "Window.h"
#include <iostream>

namespace Fiea::GameEngine
{
	std::unordered_map<int, bool> InputManager::keys;
	std::unordered_map<int, bool> InputManager::buttons;

	void InputManager::SetupKeyInputs(Window& window)
	{
		glfwSetKeyCallback(window.getWindow(), InputManager::callback);
		glfwSetMouseButtonCallback(window.getWindow(), InputManager::mouseButtonCallBack);

		// check for controller connection
		_controller.contollerConnected = glfwJoystickIsGamepad(GLFW_JOYSTICK_1);// glfwJoystickPresent(GLFW_JOYSTICK_1);	// status of joystick 1 (1 = true, 0 = false)

		if (_controller.contollerConnected == 1)
		{
			
			_controller.axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &_controller.axesCount);    // number of axes available = axesCount
			//std::cout << "Number of axes avaiable: " << axesCount << std::endl;

			//std::cout << "Left Stick X Axis: " << axes[0] << std::endl;    // -1 = left, 1 = right
			//std::cout << "Left Stick Y Axis: " << axes[1] << std::endl;    // -1 = down, 1 = right
			//std::cout << "Right Stick X Axis: " << axes[2] << std::endl;
			//std::cout << "Right Stick Y Axis: " << axes[3] << std::endl;
			//std::cout << "Left Trigger/L2: " << axes[4] << std::endl;    // fully pressed is 1, not pressed is -1
			//std::cout << "Right Trigger/R2: " << axes[5] << std::endl;

			_controller.cButtons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &_controller.cButtonCount);

			//_controller.controllerName = glfwGetJoystickName(GLFW_JOYSTICK_1); // "Xbox Controller"
			_controller.controllerName = glfwGetGamepadName(GLFW_JOYSTICK_1); // "XInput Gamepad (GLFW)"

			glfwGetGamepadState(GLFW_JOYSTICK_1, &_controller.state);	// retrieve the gamepad state of the joystick
		}
	}

	/**
	 * @brief GetKeyUp checks if Key is pressed
	 * @param btn
	 * @return true if pressed
	*/
	bool InputManager::GetKeyDown(int key)
	{
		return keys[key];
	}

	/**
	 * @brief GetKeyUp checks if Key is released
	 * @param btn
	 * @return true if not pressed
	*/
	bool InputManager::GetKeyUp(int key)
	{
		return !keys[key];
	}

	/**
	 * @brief GetMouseButtonUp checks if mouse button is pressed
	 * @param btn
	 * @return true if pressed
	*/
	bool InputManager::GetMouseButtonDown(int btn)
	{
		return buttons[btn];
	}

	/**
	 * @brief GetMouseButtonUp checks if mouse button is release
	 * @param btn 
	 * @return true if not pressed
	*/
	bool InputManager::GetMouseButtonUp(int btn)
	{
		return !buttons[btn];
	}

	/**
	 * @brief callback method for key events
	*/
	void InputManager::callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
			keys[key] = true;
		else
			keys[key] = false;
	}

	/**
	 * @brief callback method for mouse button events
	*/
	void InputManager::mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
			buttons[button] = true;
		else
			buttons[button] = false;
	}
}
