// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API ABasicPC : public APlayerController
{
	GENERATED_BODY()
public:
	ABasicPC();

	virtual void BeginPlay() override;

	void ShowItemToolTip(bool Show);

	void SetItemToolTipName(FString ItemName);

	class UItemToolTip* ItemToolTip;
	class UInventoryBase* InventoryWidget;

	void ToggleInventory();
};
