// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnDeath.AddUObject(this, &ABaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ABaseCharacter::OnHealthChanged);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ABaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero())
	{
		return 0.0f;
	}
	
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ABaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ABaseCharacter::OnHealthChanged(float Health, float Delta)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), Health)));
}


