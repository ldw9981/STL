// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_ChildActorCollision.generated.h"

/**
 * Actor의 특정 Socket(+Bone)에 AttachedActor를 찾아 컬리전 상태를 바꿈 , Begin: SetActorEnableCollision(true) -> End:SetActorEnableCollision(false)
 */
UCLASS(Blueprintable)
class MNB_API UAnimNotify_ChildActorCollision : public UAnimNotifyState
{
	GENERATED_BODY()
protected:
	TArray<class AActor*> TargetActor;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};
