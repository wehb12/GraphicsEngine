#pragma once

#include <Input/InputKeyEnum.h>

#include <functional>
#include <map>
#include <memory>
#include <vector>

using KeyBindDelegate = std::function<void(void)>;

class IInputManager
{
private:
	IInputManager();

public:
	static inline std::shared_ptr<IInputManager> Get()
	{
		return InputManagerSingleton ? InputManagerSingleton : (InputManagerSingleton = std::shared_ptr<IInputManager>(new IInputManager()));
	}

	// Bind a delgate made from a class and a pointer to a member method
	template <class DelegatorType>
	inline void BindDelegate(const EInputKey& KeyToBindTo, DelegatorType* Delegator, void (DelegatorType::* DelegatorMethod)(void))
	{
		BindDelegate(KeyToBindTo, (const KeyBindDelegate&)std::bind(DelegatorMethod, Delegator));
	}

	void BindDelegate(const EInputKey& KeyToBindTo, const KeyBindDelegate& Delegate);

	void HandleKeyPress(const EInputKey& Key);

	bool IsKeyPressed(const EInputKey& Key);

	// OR together any number of keys to see if any are pressed
	template <typename... InputKeyType>
	inline bool IsKeyPressed(const EInputKey& Key, InputKeyType... InputKeys)
	{
		return IsKeyPressed(Key) || IsKeyPressed(InputKeys...);
	}


private:
	static std::shared_ptr<IInputManager> InputManagerSingleton;

	std::map<EInputKey, std::vector<KeyBindDelegate>> KeyBindingsMap;

	void* GraphicsWindow;
};
