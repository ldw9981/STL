// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomDamageType/AnyDamageType.h"
#include "RadialDamageType.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API URadialDamageType : public UAnyDamageType
{
	GENERATED_BODY()
public:
	URadialDamageType()
	{
		DamageEventType = EDamageEventType::Radial;
	}
};
