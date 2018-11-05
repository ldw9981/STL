// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UWeaponComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

	UWeaponComponent();
public:
	int BulletPerMagazine = 30;		// 탄창 하나에 들어가는 총알 개수
	int BilletCountinMagazine = 30;	// 현재 탄창에 들어있는 총알 개수
	int TotalBulletCount = 120;		// 현재 가진 총알 개수
	float Damage = 30.0f;

	bool IsEmptyBullet();
	bool DecreaseMagazine();
	bool IsEmptyMagazine();
	bool IsFullMegazine();
	bool ReloadComplete();
	bool UseBullet();
	float GetDamage() { return Damage;  }

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
