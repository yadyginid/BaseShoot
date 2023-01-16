// Copyright Epic Games, Inc. All Rights Reserved.


#include "BaseShootGameModeBase.h"
#include "BaseShootUtils.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterPlayerController.h"
#include "Components/RespawnComponent.h"
#include "UI/BaseHUD.h"

ABaseShootGameModeBase::ABaseShootGameModeBase()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = ACharacterPlayerController::StaticClass();
	HUDClass = ABaseHUD::StaticClass();
}

void ABaseShootGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

void ABaseShootGameModeBase::Killed(AController* Controller, AController* VictimController)
{
	StartRespawn(VictimController);
}

void ABaseShootGameModeBase::StartRespawn(AController* Controller)
{
	const auto RespawnComponent = BaseShootUtils::GetPlayerComponent<URespawnComponent>(Controller);
	if (!RespawnComponent) return;

	RespawnComponent->Respawn();
}

void ABaseShootGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}
	RestartPlayer(Controller);
}
