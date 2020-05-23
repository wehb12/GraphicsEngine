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

	template <typename... DelegateParamTypes>
	inline void BindDelegate(const EInputKey& KeyToBindTo, DelegateParamTypes... DelegateParams)
	{
		BindDelegate(KeyToBindTo, (const KeyBindDelegate&)std::bind(DelegateParams...));
	}

	void BindDelegate(const EInputKey& KeyToBindTo, const KeyBindDelegate& Delegate);

	void HandleKeyPress(const EInputKey& Key);

private:
	static std::shared_ptr<IInputManager> InputManagerSingleton;

	std::map<EInputKey, std::vector<KeyBindDelegate>> KeyBindingsMap;
};
