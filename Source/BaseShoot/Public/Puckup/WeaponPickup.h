// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShoot/Public/CoreTypes.h"
#include "Puckup/BasePickup.h"
#include "WeaponPickup.generated.h"

class ABaseWeapon;

UCLASS()
class BASESHOOT_API AWeaponPickup : public ABasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FWeaponData WeaponData;
	
private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
