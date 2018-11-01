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
	int BulletPerMagazine = 30;
	int BilletCountinMagazine = 30;
	int TotalBulletCount = 120;
	
	void ChangeMagazine();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
