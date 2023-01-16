// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SpectatorWidget.h"
#include "BaseShootUtils.h"
#include "Components/RespawnComponent.h"

bool USpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = BaseShootUtils::GetPlayerComponent<URespawnComponent>(GetOwningPlayerPawn());
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}
