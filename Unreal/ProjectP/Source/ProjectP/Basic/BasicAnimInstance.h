// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasicAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UBasicAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:	

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsSprint = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsCrouched = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float AimPitch=0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float AimYaw=0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIronSight = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	float JumpVelocityZ = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsFalling = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsReload = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsFire = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bLeftLean = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bRightLean = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	//float TargetAngle = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentAngle = 0;

	UFUNCTION()
	void AnimNotify_ReloadComplete(UAnimNotify* Notify);	// "AnimNotify_" PREFIX사용해야함
};
