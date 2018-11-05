// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UWeaponComponent::IsEmptyBullet()
{
	if (TotalBulletCount <= 0)
		return true;

	return false;
}

// 총알 없으면 false,있으면 트루
bool UWeaponComponent::DecreaseMagazine()
{
	if (IsEmptyMagazine())
		return false;

	BilletCountinMagazine--;
	UE_LOG(LogClass, Warning, TEXT("TotalBulletCount: %d   BilletCountinMagazine:%d "), TotalBulletCount, BilletCountinMagazine);
	return true;
}

bool UWeaponComponent::IsEmptyMagazine()
{
	if (BilletCountinMagazine <= 0)
		return true;

	return false;
}

bool UWeaponComponent::IsFullMegazine()
{
	if (BilletCountinMagazine == BulletPerMagazine)
		return true;

	return false;
}

bool UWeaponComponent::ReloadComplete()
{
	if (IsFullMegazine())
		return false;

	if (IsEmptyBullet())
		return false;

	int Count = FMath::Min( BulletPerMagazine - BilletCountinMagazine, TotalBulletCount);
	BilletCountinMagazine += Count;
	TotalBulletCount -= Count;

	UE_LOG(LogClass, Warning, TEXT("TotalBulletCount: %d   BilletCountinMagazine:%d "), TotalBulletCount, BilletCountinMagazine);
	return true;
}

inline bool UWeaponComponent::UseBullet()
{
	if (BilletCountinMagazine > 0)
	{
		BilletCountinMagazine--;
		return true;
	}
	return false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
