// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class BASESHOOT_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponProjectile();
	
	void SetShootDirection(const FVector Direction) {ShootDirection = Direction;};

protected:
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float DamageRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float DamageAmount = 50.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	bool DoFullDamage = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float LifeSeconds = 5.0f;
	
	virtual void BeginPlay() override;

private:
	FVector ShootDirection;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
