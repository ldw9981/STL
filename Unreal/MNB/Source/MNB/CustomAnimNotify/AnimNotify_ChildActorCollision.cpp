// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_ChildActorCollision.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkeletalMeshSocket.h"

void UAnimNotify_ChildActorCollision::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	AActor* Actor = MeshComp->GetOwner();
	if (!Actor)
	{
		return;
	}
	TArray<class AActor*> ChildActors;
	Actor->GetAllChildActors(ChildActors);

	for (auto ChildActor : ChildActors)
	{
		if (ChildActor->GetParentComponent()->GetAttachSocketName() == SocketName)
		{
			TargetActor.Add(ChildActor);
			ChildActor->SetActorEnableCollision(true);
			UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
		}
	}
}

void UAnimNotify_ChildActorCollision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	for (auto Actor : TargetActor)
	{
		Actor->SetActorEnableCollision(false);		
		UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
	}
	TargetActor.Empty();
}
