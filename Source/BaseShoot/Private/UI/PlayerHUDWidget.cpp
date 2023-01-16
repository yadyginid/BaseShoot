// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUDWidget.h"
#include <Components/HealthComponent.h>
#include "BaseShootUtils.h"
#include "Components/WeaponComponent.h"

float UPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = BaseShootUtils::GetCharacterComponent<UHealthComponent>(GetOwningPlayerPawn());
	
	if (!HealthComponent)
	{
		return 0.0f;
	}

	return HealthComponent->GetHealthPercent();
}

bool UPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	const auto WeaponComponent = BaseShootUtils::GetCharacterComponent<UWeaponComponent>(GetOwningPlayerPawn());;
	
	if (!WeaponComponent)
	{
		return false;
	}

	return WeaponComponent->GetWeaponUIData(WeaponUIData);
}

bool UPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& WeaponAmmoData) const
{
	const auto WeaponComponent = BaseShootUtils::GetCharacterComponent<UWeaponComponent>(GetOwningPlayerPawn());
	
	if (!WeaponComponent)
	{
		return false;
	}

	return WeaponComponent->GetWeaponAmmoData(WeaponAmmoData);
}

bool UPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = BaseShootUtils::GetCharacterComponent<UHealthComponent>(GetOwningPlayerPawn());;
	return HealthComponent && !HealthComponent->IsDead();
}

bool UPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}