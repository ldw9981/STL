// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UWeaponComponent::GetDamage()
{
	return Damage;
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UWeaponComponent::IsHaveBullet()
{
	return TotalBulletCount <= 0 ? false : true;
}

void UWeaponComponent::ReloadComplete()
{
	//�Ѿ��� ���°� 
	if (!IsHaveBullet())
	{
		return;
	}

	//���� �Ǿ� �� �Ѿ� ��
	int AddBullet = BulletPerMagazine - BulletCountinMagazine;

	//���� �Ѿ��� ���� �� �Ѿ˺��� ���� ���
	if (TotalBulletCount <= AddBullet)
	{
		BulletCountinMagazine += TotalBulletCount;
		TotalBulletCount = 0;
	}
	else
	{
		//���� �Ѿ˿��� ���� �� �Ѿ��� ����
		TotalBulletCount -= AddBullet;
		BulletCountinMagazine = BulletPerMagazine;
	}
}

bool UWeaponComponent::UseBullet()
{
	if (BulletCountinMagazine >  0)
	{
		BulletCountinMagazine--;
		return true;
	}
	else
	{
		return false;
	}
}

