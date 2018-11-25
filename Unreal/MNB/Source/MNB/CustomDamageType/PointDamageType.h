// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "CustomDamageType/CustomDamageType.h"
#include "PointDamageType.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UPointDamageType : public UCustomDamageType
{
	GENERATED_BODY()
public:
	UPointDamageType()
	{
		CustomDamageEventType = ECustomDamageEventType::Point;
	}

};
