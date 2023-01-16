// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RespawnComponent.h"
#include "BaseShoot/BaseShootGameModeBase.h"

URespawnComponent::URespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URespawnComponent::Respawn() const
{
	if (!GetWorld()) return;
	
	const auto GameMode = Cast<ABaseShootGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	GameMode->RespawnRequest(Cast<AController>(GetOwner()));
}
