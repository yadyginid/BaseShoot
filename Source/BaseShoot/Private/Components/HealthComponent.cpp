// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &UHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UHealthComponent::OnTakeRadialDamage);
	}
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	ApplyDamage(Damage);
}

void UHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	ApplyDamage(Damage);
}

void UHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName,
                       FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	ApplyDamage(Damage);
}

void UHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

void UHealthComponent::ApplyDamage(float Damage)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}
