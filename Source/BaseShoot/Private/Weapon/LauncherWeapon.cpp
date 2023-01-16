// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LauncherWeapon.h"
#include "Weapon/WeaponProjectile.h"

void ALauncherWeapon::StartFire()
{
	MakeShot();
}

void ALauncherWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		return;
	}

	FVector TraceStart;
	FVector TraceEnd;

	if (!GetTraceData(TraceStart, TraceEnd))
	{
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	FVector Direction = (EndPoint - GetMuzzleWeaponLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWeaponLocation());

	AWeaponProjectile* Projectile = GetWorld()->SpawnActorDeferred<AWeaponProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile)
	{
		Projectile->SetShootDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}
	
	DecreaseAmmo();
}
