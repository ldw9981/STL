// Fill out your copyright notice in the Description page of Project Settings.

#include "BDGameInstance.h"
#include "Items/InventorySystem.h"

void UBDGameInstance::Init()
{
	Inventory = NewObject<UInventorySystem>(this);

	//For Test
	if (UserID.Len() == 0)
	{
		UserID = FString::Printf(TEXT("Noname%d"), FMath::RandRange(0, 1000));
	}
}
