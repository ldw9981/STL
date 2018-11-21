// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimInstance.h"
#include "CustomComponents/CharacterStateComponent.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterState CurrentState = ECharacterState::Normal;

	UFUNCTION()
	void AnimNotify_NotifyTest(UAnimNotify* Notify);
};
