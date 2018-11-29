// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_WeaponCollision.generated.h"

/**
 * 지정한 태그를 갖는 스케레탈 메쉬의 자식 액터의 컬리전 상태를 변경한다.
 */
UCLASS()
class MNB_API UAnimNotify_WeaponCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	/** Array of tags that can be used for grouping and categorizing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = WeaponCollision)
	TArray<FName> TargetTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = WeaponCollision)
	bool EnableCollision=true;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
