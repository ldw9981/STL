// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DAY05_R_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsSpeed;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Angle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ForwardValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RightValue;
};
