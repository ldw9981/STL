// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomWidget/ItemSlotBase.h"
#include "InventoryBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UInventoryBase : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeStruct() override;

	TArray<class UItemSlotBase*> Slots;
};
