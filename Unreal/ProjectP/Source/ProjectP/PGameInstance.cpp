// Fill out your copyright notice in the Description page of Project Settings.

#include "PGameInstance.h"
#include "Items/InventorySystem.h"

void UPGameInstance::Init()
{
	Inventory = NewObject<UInventorySystem>(this);
}
