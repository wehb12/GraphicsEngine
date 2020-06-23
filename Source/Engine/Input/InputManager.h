#pragma once

#include <Input/InputKeyEnum.h>

#include <functional>
#include <map>
#include <memory>
#include <vector>

using KeyPressDelegate = std::function<void(void)>;
// MouseBindDelegate takes in the CHANGE in X and Y coordinates
using MouseMoveDelegate = std::function<void(double, double)>;
using MouseScrollDelegate = std::function<void(double)>;

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
		BindDelegate(KeyToBindTo, (const KeyPressDelegate&)std::bind(DelegatorMethod, Delegator));
	}

	// Bind a delgate made from a lambda
	inline void BindDelegate(const EInputKey& KeyToBindTo, void (DelegatorLambda)(void))
	{
		BindDelegate(KeyToBindTo, (const KeyPressDelegate&)std::bind(DelegatorLambda));
	}

	void BindDelegate(const EInputKey& KeyToBindTo, const KeyPressDelegate& Delegate)
	{
		KeyBindingsMap[KeyToBindTo].push_back(Delegate);
	}

	// Bind a delgate made from a class and a pointer to a member method
	template <class DelegatorType>
	inline void BindDelegate(DelegatorType* Delegator, void (DelegatorType::* DelegatorMethod)(double, double))
	{
		BindDelegate((const MouseMoveDelegate&)std::bind(DelegatorMethod, Delegator, std::placeholders::_1, std::placeholders::_2));
	}

	void BindDelegate(const MouseMoveDelegate& Delegate)
	{
		MouseMoveBindings.push_back(Delegate);
	}

	// Bind a delgate made from a class and a pointer to a member method
	template <class DelegatorType>
	inline void BindDelegate(DelegatorType* Delegator, void (DelegatorType::* DelegatorMethod)(double))
	{
		BindDelegate((const MouseScrollDelegate&)std::bind(DelegatorMethod, Delegator, std::placeholders::_1));
	}

	void BindDelegate(const MouseScrollDelegate& Delegate)
	{
		MouseScrollBindings.push_back(Delegate);
	}

	void HandleKeyPress(const EInputKey& Key);

	void HandleMouseMove(double XPosition, double YPosition);

	void HandleMouseScroll(double ScrollPosition);

	bool IsKeyPressed(const EInputKey& Key);

	// OR together any number of keys to see if any are pressed
	template <typename... InputKeyType>
	inline bool IsKeyPressed(const EInputKey& Key, InputKeyType... InputKeys)
	{
		return IsKeyPressed(Key) || IsKeyPressed(InputKeys...);
	}

private:
	static std::shared_ptr<IInputManager> InputManagerSingleton;

	std::map<EInputKey, std::vector<KeyPressDelegate>> KeyBindingsMap;
	std::vector<MouseMoveDelegate> MouseMoveBindings;
	std::vector<MouseScrollDelegate> MouseScrollBindings;

	double LastMouseXPosition = 0.0;
	double LastMouseYPosition = 0.0;

	void* GraphicsWindow = nullptr;
};
