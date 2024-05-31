#pragma once
#include "InputManager.h"

#define GAMEPAD_LEFT_AXIS_RIGHT (InputManager::_controller.state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.5f)
#define GAMEPAD_LEFT_AXIS_LEFT (InputManager::_controller.state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.5f)
#define GAMEPAD_LEFT_AXIS_UP (InputManager::_controller.state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -0.5f)
#define GAMEPAD_LEFT_AXIS_DOWN (InputManager::_controller.state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > 0.5f)
#define GAMEPAD_BUTTON_X InputManager::_controller.state.buttons[GLFW_GAMEPAD_BUTTON_X]
#define GAMEPAD_BUTTON_A InputManager::_controller.state.buttons[GLFW_GAMEPAD_BUTTON_A]
#define GAMEPAD_BUTTON_B InputManager::_controller.state.buttons[GLFW_GAMEPAD_BUTTON_B]
#define GAMEPAD_BUTTON_Y InputManager::_controller.state.buttons[GLFW_GAMEPAD_BUTTON_Y]
#define GAMEPAD_BUTTON_RIGHT_BUMPER InputManager::_controller.state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]
#define GAMEPAD_BUTTON_LEFT_BUMPER InputManager::_controller.state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]
