// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemDataTable.h"
#include "UObject/NoExportTypes.h"
#include "Items/ItemDataTable.h"
#include "InventorySystem.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTP_API UInventorySystem : public UObject
{
	GENERATED_BODY()
public:
	TArray<FItemDataTable> ItemList;

	bool AddItem(FItemDataTable NewItem);
	bool UseItem(int Index);
	bool DropItem(int Index);
	int GetSameItemIndex(FItemDataTable NewItem);
};
