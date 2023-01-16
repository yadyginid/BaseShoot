// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"

#include "Animation/EquipFinishedAnimNotify.h"
#include "Animation/ReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "BaseShoot/AnimUtils.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}

	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void UWeaponComponent::StartFire()
{
	if (!CanFire())
	{
		return;
	}

	CurrentWeapon->StartFire();
}

void UWeaponComponent::StopFire()
{
	if (!CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->StopFire();
}

void UWeaponComponent::NextWeapon()
{
	if (!CanEquip())
	{
		return;
	}

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::Reload()
{
	ChangeClip();
}

bool UWeaponComponent::GetWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	if (!CurrentWeapon)
	{
		return false;
	}

	WeaponUIData = CurrentWeapon->GetWeaponUIData();
	return true;
}

bool UWeaponComponent::GetWeaponAmmoData(FAmmoData& WeaponAmmoData) const
{
	if (!CurrentWeapon)
	{
		return false;
	}

	WeaponAmmoData = CurrentWeapon->GetAmmoData();
	return true;
}

bool UWeaponComponent::TryAddWeapon(FWeaponData WeaponData)
{
	return SpawnWeapon(WeaponData);
}


void UWeaponComponent::SpawnWeapons()
{
	if (!GetWorld())
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	for (auto WeaponData : WeaponsData)
	{
		auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponData.WeaponClass);

		if (!Weapon)
		{
			return;
		}

		Weapon->OnClipEmpty.AddUObject(this, &UWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UWeaponComponent::AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* SceneComponent,
                                            const FName& SocketName)
{
	if (!Weapon)
	{
		return;
	}

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

	EquipAnimInProgress = true;
	const auto CurrenWeaponData = WeaponsData.FindByPredicate([&](const FWeaponData& Data){return Data.WeaponClass == CurrentWeapon->GetClass();});
	CurrentReloadAnimMontage = CurrenWeaponData ? CurrenWeaponData->ReloadAnimMontage : nullptr;

	PlayAnimMontage(EquipAnimMontage);
}

void UWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	Character->PlayAnimMontage(Animation);
}

void UWeaponComponent::InitAnimations()
{
	if (!EquipAnimMontage)
	{
		return;
	}

	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("EquipFinishedNotify not found"))
	}

	for (auto Weapon : WeaponsData)
	{
		auto ReloadFinishedAnimNotify = AnimUtils::FindNotifyByClass<UReloadFinishedAnimNotify>(
			Weapon.ReloadAnimMontage);
		if (!ReloadFinishedAnimNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("ReloadFinishedAnimNotify not found"))
		}

		ReloadFinishedAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnReloadFinished);
	}
}

void UWeaponComponent::OnEquipFinished(USkeletalMeshComponent* SkeletalMeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	if (Character->GetMesh() != SkeletalMeshComponent)
	{
		return;
	}

	EquipAnimInProgress = false;
}

void UWeaponComponent::OnReloadFinished(USkeletalMeshComponent* SkeletalMeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	if (Character->GetMesh() != SkeletalMeshComponent)
	{
		return;
	}

	ReloadAnimInProgress = false;
}

bool UWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UWeaponComponent::CanReload() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void UWeaponComponent::OnEmptyClip()
{
	ChangeClip();
}

void UWeaponComponent::ChangeClip()
{
	if (!CanReload())
	{
		return;
	}

	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();

	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

bool UWeaponComponent::SpawnWeapon(FWeaponData WeaponData)
{
	if (!GetWorld())
	{
		return false;
	}
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return false;
	}

	const auto CurrenWeaponData = WeaponsData.FindByPredicate([&](const FWeaponData& Data){return Data.WeaponClass == WeaponData.WeaponClass;});
	
	if (CurrenWeaponData)
	{
		return false;
	}
	
	auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponData.WeaponClass);
	if (!Weapon)
	{
		return false;
	}

	Weapon->OnClipEmpty.AddUObject(this, &UWeaponComponent::OnEmptyClip);
	Weapon->SetOwner(Character);
	Weapons.Add(Weapon);
	
	auto ReloadFinishedAnimNotify = AnimUtils::FindNotifyByClass<UReloadFinishedAnimNotify>(
			WeaponData.ReloadAnimMontage);
	if (!ReloadFinishedAnimNotify)
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("ReloadFinishedAnimNotify not found"))
		return false;
	}

	ReloadFinishedAnimNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnReloadFinished);
	WeaponsData.Add(WeaponData);
	
	return true;
}
