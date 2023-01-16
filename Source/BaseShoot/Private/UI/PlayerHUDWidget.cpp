// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUDWidget.h"
#include <Components/HealthComponent.h>
#include "BaseShootUtils.h"

float UPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = BaseShootUtils::GetCharacterComponent<UHealthComponent>(GetOwningPlayerPawn());
	
	if (!HealthComponent)
	{
		return 0.0f;
	}

	return HealthComponent->GetHealthPercent();
}
