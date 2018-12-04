// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	class UInventorySystem* Inventory;
public:
	virtual void Init() override;
};
