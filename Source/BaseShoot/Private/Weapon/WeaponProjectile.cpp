// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponProjectile.h"

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AWeaponProjectile::AWeaponProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;

	CollisionComponent->OnComponentHit.AddDynamic(this, &AWeaponProjectile::OnProjectileHit);
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);

	SetLifeSpan(LifeSeconds);
}

void AWeaponProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld())
	{
		return;
	}

	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		DamageAmount,
		GetActorLocation(),
		DamageRadius,
		nullptr,
		{GetOwner()},
		this,
		nullptr,
		DoFullDamage
		);

	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
	
	Destroy();
}


