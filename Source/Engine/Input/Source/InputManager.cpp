#include "Input/InputManager.h"

#define GLFWKEYMAP_INPUTMANAGERPRIVATE
#include "Input/Platform/GLFWKeyMap.h"
#undef GLFWKEYMAP_INPUTMANAGERPRIVATE
#include "Graphics/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::shared_ptr<IInputManager> IInputManager::InputManagerSingleton;

void GLFWKeyPressDelegate(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
void GLFWMouseMoveDelegate(GLFWwindow* Window, double XPosition, double YPosition);
void GLFWMouseScrollDelegate(GLFWwindow* Window, double XPosition, double YPosition);

IInputManager::IInputManager()
{
	GraphicsWindow = GWindow::Get()->GetRawWindowPtr();
	glfwSetKeyCallback((GLFWwindow*)GraphicsWindow, GLFWKeyPressDelegate);

	glfwGetCursorPos((GLFWwindow*)GraphicsWindow, &LastMouseXPosition, &LastMouseYPosition);
	glfwSetCursorPosCallback((GLFWwindow*)GraphicsWindow, GLFWMouseMoveDelegate);
	glfwSetScrollCallback((GLFWwindow*)GraphicsWindow, GLFWMouseScrollDelegate);
}

void IInputManager::HandleKeyPress(const EInputKey& Key)
{
	auto& KeyBindingsIt = KeyBindingsMap.find(Key);
	if (KeyBindingsIt != KeyBindingsMap.end())
	{
		std::vector<KeyPressDelegate> KeyBindDelegates = KeyBindingsIt->second;

		for (const KeyPressDelegate& Delegate : KeyBindDelegates)
		{
			Delegate();
		}
	}
}

void IInputManager::HandleMouseMove(double XPosition, double YPosition)
{
	for (const MouseMoveDelegate& Delegate : MouseMoveBindings)
	{
		Delegate(XPosition - LastMouseXPosition, YPosition - LastMouseYPosition);
	}

	LastMouseXPosition = XPosition;
	LastMouseYPosition = YPosition;
}

void IInputManager::HandleMouseScroll(double ScrollPosition)
{
	for (const MouseScrollDelegate& Delegate : MouseScrollBindings)
	{
		Delegate(ScrollPosition);
	}
}

bool IInputManager::IsKeyPressed(const EInputKey& Key)
{
	return glfwGetKey((GLFWwindow*)GWindow::Get()->GetRawWindowPtr(), GetGLFWKeyFromInputKey(Key));
}

void GLFWKeyPressDelegate(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	const EInputKey InputKey = GetKeyFromGLFWKey(Key);
	IInputManager::Get()->HandleKeyPress(InputKey);
}

void GLFWMouseMoveDelegate(GLFWwindow* Window, double XPosition, double YPosition)
{
	IInputManager::Get()->HandleMouseMove(XPosition, YPosition);
}

void GLFWMouseScrollDelegate(GLFWwindow* Window, double XPosition, double YPosition)
{
	IInputManager::Get()->HandleMouseScroll(YPosition);
}