// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_WeaponCollision.generated.h"

/**
 * ������ �±׸� ���� ���ɷ�Ż �޽��� �ڽ� ������ �ø��� ���¸� �����Ѵ�.
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
