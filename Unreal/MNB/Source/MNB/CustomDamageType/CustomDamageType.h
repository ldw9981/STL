// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "CustomDamageType.generated.h"


UENUM(BlueprintType)
enum class ECustomDamageEventType : uint8
{
	Generic = 0		UMETA(Display, "Generic"),
	Point = 1		UMETA(Display, "Point"),
	Radial = 2		UMETA(Display, "Radial"),
};

/**
 * 
 */
UCLASS()
class MNB_API UCustomDamageType : public UDamageType
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageEvent")
	ECustomDamageEventType CustomDamageEventType = ECustomDamageEventType::Generic;
};
