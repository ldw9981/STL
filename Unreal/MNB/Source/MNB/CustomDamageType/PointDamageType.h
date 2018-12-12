// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "CustomDamageType/AnyDamageType.h"
#include "PointDamageType.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UPointDamageType : public UAnyDamageType
{
	GENERATED_BODY()
public:
	UPointDamageType()
	{
		DamageEventType = EDamageEventType::Point;
	}

};
