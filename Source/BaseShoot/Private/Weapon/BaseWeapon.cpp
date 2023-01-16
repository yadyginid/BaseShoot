// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Characters/BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = DefaultAmmo;
}


void ABaseWeapon::StartFire()
{
}

void ABaseWeapon::StopFire()
{
}

void ABaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			return;
		}
		
		CurrentAmmo.Clips--;
	}

	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool ABaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}


void ABaseWeapon::MakeShot()
{
}

APlayerController* ABaseWeapon::GetPlayerController() const
{
	if (!GetWorld())
	{
		return nullptr;
	}

	const auto Player = Cast<ABaseCharacter>(GetOwner());
	if (!Player)
	{
		return nullptr;
	}

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller)
	{
		return nullptr;
	}

	return Controller;
}

bool ABaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if (!Controller)
	{
		return false;
	}
	
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

FVector ABaseWeapon::GetMuzzleWeaponLocation() const
{
	return  WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ABaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
	{
		return false;
	}
	
	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ABaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if (!GetWorld())
	{
		return;
	}
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
}

void ABaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		return;
	}
	
	CurrentAmmo.Bullets--;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast();
	}
}

bool ABaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ABaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}
