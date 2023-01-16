// Fill out your copyright notice in the Description page of Project Settings.


#include "Puckup/HealthPickup.h"

#include "BaseShootUtils.h"
#include "Components/HealthComponent.h"

bool AHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = BaseShootUtils::GetPlayerComponent<UHealthComponent>(PlayerPawn);
	if (!HealthComponent) return false;

	return HealthComponent->TryToAddHealth(HealthAmount);
}
