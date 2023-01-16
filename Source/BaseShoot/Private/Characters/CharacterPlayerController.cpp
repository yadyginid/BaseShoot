// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterPlayerController.h"

#include "Components/RespawnComponent.h"

ACharacterPlayerController::ACharacterPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<URespawnComponent>("RespawnComponent");
}

void ACharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn);
}
