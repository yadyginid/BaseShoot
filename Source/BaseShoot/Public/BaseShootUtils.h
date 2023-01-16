#pragma once

class BaseShootUtils
{
public:
	template <typename T>
	static T* GetPlayerComponent(AActor* SomeActor)
	{
		if (!SomeActor) return nullptr;

		const auto Component = SomeActor->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};
