#include "Input/InputManager.h"

#define GLFWKEYMAP_INPUTMANAGERPRIVATE
#include "Input/Platform/GLFWKeyMap.h"
#undef GLFWKEYMAP_INPUTMANAGERPRIVATE
#include "Graphics/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::shared_ptr<IInputManager> IInputManager::InputManagerSingleton;

void GLFWKeyPressDelegate(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);

IInputManager::IInputManager()
{
	glfwSetKeyCallback((GLFWwindow*)GWindow::Get()->GetRawWindowPtr(), GLFWKeyPressDelegate);
}

void IInputManager::BindDelegate(const EInputKey& KeyToBindTo, const KeyBindDelegate& Delegate)
{
	KeyBindingsMap[KeyToBindTo].push_back(Delegate);
}

void IInputManager::HandleKeyPress(const EInputKey& Key)
{
	auto& KeyBindingsIt = KeyBindingsMap.find(Key);
	if (KeyBindingsIt != KeyBindingsMap.end())
	{
		std::vector<KeyBindDelegate> KeyBindDelegates = KeyBindingsIt->second;

		for (const KeyBindDelegate& Delegate : KeyBindDelegates)
		{
			Delegate();
		}
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
