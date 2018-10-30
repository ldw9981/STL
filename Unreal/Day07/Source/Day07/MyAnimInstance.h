// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DAY07_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float ForwardValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RightValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Angle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool ExistSpeed;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	float TimeRotate = 0.3f;
	float AnglePrev=0.0f;
	float AngleTarget=0.0f;
	float TimePrev=0.0f;
};
