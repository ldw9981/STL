// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_WeaponCollision.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon/WeaponActor.h"
#include "Components/ShapeComponent.h"
void UAnimNotify_WeaponCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AActor* Actor = MeshComp->GetOwner();
	if (!Actor)
	{
		return;
	}

	TArray<AActor*> ChildActors;
	Actor->GetAllChildActors(ChildActors);		
	for (int32 index_ChildActor = 0; index_ChildActor < ChildActors.Num(); index_ChildActor++)
	{	
		AWeaponActor* WeaponActor = Cast<AWeaponActor>(ChildActors[index_ChildActor]);
		if (!WeaponActor || !WeaponActor->Equiped)
		{
			continue;
		}

		
		TArray<UActorComponent*> ShapeComponents = WeaponActor->GetComponentsByClass(UShapeComponent::StaticClass());
		for (UActorComponent* ActorComp : ShapeComponents)
		{
			UShapeComponent* Shape = Cast<UShapeComponent>(ActorComp);
			if (Shape)
			{
				//Shape->OnComponentHit.AddDynamic(this, &AProjectile::OnComponentHit);
			}
		}
		/*
		for (int32 index_TargetTags = 0; index_TargetTags < TargetTags.Num(); index_TargetTags++)
		{

		}
		WeaponActor->Tags.Find
			*/
	}

//	Actor->GetComponentsByTag()
}
