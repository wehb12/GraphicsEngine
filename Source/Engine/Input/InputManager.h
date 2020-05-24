#pragma once

#include <Input/InputKeyEnum.h>

#include <functional>
#include <map>
#include <memory>
#include <vector>

using KeyBindDelegate = std::function<void(void)>;
// MouseBindDelegate takes in the CHANGE in X and Y coordinates
using MouseBindDelegate = std::function<void(double, double)>;

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

	void BindDelegate(const EInputKey& KeyToBindTo, const KeyBindDelegate& Delegate)
	{
		KeyBindingsMap[KeyToBindTo].push_back(Delegate);
	}

	// Bind a delgate made from a class and a pointer to a member method
	template <class DelegatorType>
	inline void BindDelegate(DelegatorType* Delegator, void (DelegatorType::* DelegatorMethod)(double, double))
	{
		BindDelegate((const MouseBindDelegate&)std::bind(DelegatorMethod, Delegator, std::placeholders::_1, std::placeholders::_2));
	}

	void BindDelegate(const MouseBindDelegate& Delegate)
	{
		MouseBindings.push_back(Delegate);
	}

	void HandleKeyPress(const EInputKey& Key);

	void HandleMouseMove(double XPosition, double YPosition);

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
	std::vector<MouseBindDelegate> MouseBindings;

	double LastMouseXPosition = 0.0;
	double LastMouseYPosition = 0.0;

	void* GraphicsWindow = nullptr;
};
