// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemToolTip.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UItemToolTip : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextBlock* ItemName;
public:
	virtual void NativeConstruct() override;
};
