// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/DamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ADamageActor::ADamageActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
}

void ADamageActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, nullptr, {}, this, nullptr,
										DoFullDamage);
}

