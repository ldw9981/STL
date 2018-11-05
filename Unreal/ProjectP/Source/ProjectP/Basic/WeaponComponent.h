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
	int BulletPerMagazine = 30;		// źâ �ϳ��� ���� �Ѿ� ����
	int BilletCountinMagazine = 30;	// ���� źâ�� ����ִ� �Ѿ� ����
	int TotalBulletCount = 120;		// ���� ���� �Ѿ� ����
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
