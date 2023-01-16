#pragma once

class BaseShootUtils
{
public:
	template <typename T>
	static T* GetCharacterComponent(APawn* Character)
	{
		if (!Character)
		{
			return nullptr;
		}

		const auto Component = Character->GetComponentByClass(T::StaticClass());
		return  Cast<T>(Component);
	}
};
