// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AdamAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UAdamAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		float Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		float Speed;
};
