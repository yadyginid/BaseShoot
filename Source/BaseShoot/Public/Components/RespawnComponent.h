// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASESHOOT_API URespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URespawnComponent();

	void Respawn() const;
};
