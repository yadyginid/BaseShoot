// Fill out your copyright notice in the Description page of Project Settings.


#include "Puckup/BasePickup.h"
#include "Components/SphereComponent.h"

ABasePickup::ABasePickup()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);
}

void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	
	for (const auto OverlapPawn : OverlappingPawns)
	{
		if (GivePickupTo(OverlapPawn))
		{
			PickupWasTaken();
			break;
		}
	}
}

void ABasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Pawn = Cast<APawn>(OtherActor);
	if (GivePickupTo(Pawn))
	{
		PickupWasTaken();
	}
	else if (Pawn)
	{
		OverlappingPawns.Add(Pawn);
	}
}

void ABasePickup::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	const auto Pawn = Cast<APawn>(OtherActor);
	OverlappingPawns.Remove(Pawn);
}

void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABasePickup::GivePickupTo(APawn* OtherActor)
{
	return false;
}

void ABasePickup::PickupWasTaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
	}

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ABasePickup::Respawn, RespawnTime);
}

void ABasePickup::Respawn()
{
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}
