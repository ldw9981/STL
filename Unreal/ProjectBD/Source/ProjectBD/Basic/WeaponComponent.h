// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UWeaponComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	UWeaponComponent();

	int BulletPerMagazine = 30; // ��źâ�� ���� �Ѿ� ��
	int BulletCountinMagazine = 30;  //���� źâ�� �ִ� �Ѿ� ��
	int TotalBulletCount = 120; // ��ü �Ѿ˼�

	//���� �Ѿ��� �ִ��� Ȯ��
	bool IsHaveBullet();

	//������ �Ϸ�
	void ReloadComplete();

	//�Ѿ� ���
	bool UseBullet();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	float Damage = 1.0f;

	float GetDamage();

};
