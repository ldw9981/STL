// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Unknown = 0		UMETA(Display = "Unknown"),
	Consume = 1		UMETA(Display = "Consume"),
	Equip = 2		UMETA(Display = "Equip")
};

UENUM(BlueprintType)
enum class EItemEquipType : uint8
{
	NotUse = 0		UMETA(Display = "NotUse"),
	WeaponRightHand = 1		UMETA(Display = "WeaponRightHand"),
	Weapon = 2		UMETA(Display = "Weapon")
};

USTRUCT(BlueprintType)
struct PROJECTP_API FItemDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int	ItemIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString	ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemType ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemEquipType	ItemEquipType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float 	ItemFloat01;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int	ItemCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UTexture2D>	ItemThumnail;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UStaticMesh> ItemMesh;
};
