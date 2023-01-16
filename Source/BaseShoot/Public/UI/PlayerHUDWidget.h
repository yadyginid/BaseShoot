// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseShoot/Public/CoreTypes.h"
#include "PlayerHUDWidget.generated.h"

UCLASS()
class BASESHOOT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UI")
		float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool GetWeaponUIData(FWeaponUIData& WeaponUIData) const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool GetWeaponAmmoData(FAmmoData& WeaponAmmoData) const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool IsPlayerAlive() const;
	
	UFUNCTION(BlueprintCallable, Category="UI")
	bool IsPlayerSpectating() const;
};
