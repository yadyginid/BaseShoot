// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/RiffeWeapon.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"

void ARiffeWeapon::StartFire()
{
	if (!GetWorld())
	{
		return;
	}
	
	GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ARiffeWeapon::MakeShot, TimeBetweenShoots, true);
	MakeShot();
}

void ARiffeWeapon::StopFire()
{
	if (!GetWorld())
	{
		return;
	}
	
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void ARiffeWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart;
	FVector TraceEnd;

	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector MuzzleWeaponLocation = GetMuzzleWeaponLocation();
	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), MuzzleWeaponLocation, HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24.0f, FColor::Red, false, 5.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), MuzzleWeaponLocation, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}
	
	DecreaseAmmo();
}

bool ARiffeWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
	{
		return false;
	}
	
	TraceStart = ViewLocation;
	const float HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ARiffeWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor)
	{
		return;
	}

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
