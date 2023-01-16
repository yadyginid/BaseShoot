// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseHUD.h"

#include "Blueprint/UserWidget.h"

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (!PlayerHUDWidget)
	{
		return;
	}

	PlayerHUDWidget->AddToViewport();
}
