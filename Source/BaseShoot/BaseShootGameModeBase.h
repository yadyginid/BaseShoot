// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseShootGameModeBase.generated.h"

UCLASS()
class BASESHOOT_API ABaseShootGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseShootGameModeBase();
	
	void RespawnRequest(AController* Controller);
	void Killed(AController* Controller, AController* VictimController);
	void StartRespawn(AController* Controller);

private:
	void ResetOnePlayer(AController* Controller);
};
