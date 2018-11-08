// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_ZombieWalkSpeed.h"
#include "Zombie/ZombieCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_ZombieWalkSpeed::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	AZombieCharacter* ZombieCharacter = Cast< AZombieCharacter>(MeshComp->GetOwner());
	if (!ZombieCharacter)
	{
		return;
	}

	ZombieCharacter->GetCharacterMovement()->MaxWalkSpeed = SlowSpeed;
	if (ZombieCharacter->CurrentState == EZombieState::Chase)
	{
		ZombieCharacter->GetCharacterMovement()->MaxWalkSpeed = ZombieCharacter->RunSpeed;
	}
}

void UAnimNotifyState_ZombieWalkSpeed::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
}

void UAnimNotifyState_ZombieWalkSpeed::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AZombieCharacter* ZombieCharacter = Cast< AZombieCharacter>(MeshComp->GetOwner());
	if (!ZombieCharacter)
	{
		return;
	}

	ZombieCharacter->GetCharacterMovement()->MaxWalkSpeed = ZombieCharacter->WalkSpeed;
}
