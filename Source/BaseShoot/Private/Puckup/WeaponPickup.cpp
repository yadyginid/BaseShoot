// Fill out your copyright notice in the Description page of Project Settings.


#include "Puckup/WeaponPickup.h"

#include "BaseShootUtils.h"
#include "Components/WeaponComponent.h"

bool AWeaponPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto WeaponComponent = BaseShootUtils::GetPlayerComponent<UWeaponComponent>(PlayerPawn);
	if (!WeaponComponent)
	{
		return false;
	}

	return WeaponComponent->TryAddWeapon(WeaponData);
}
