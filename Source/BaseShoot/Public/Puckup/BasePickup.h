// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

class USphereComponent;

UCLASS()
class BASESHOOT_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 7.0f;
	
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TArray<APawn*> OverlappingPawns;
	
	FTimerHandle RespawnTimerHandle;
	
	virtual bool GivePickupTo(APawn* PlayerPawn);
	void PickupWasTaken();
	void Respawn();
};
