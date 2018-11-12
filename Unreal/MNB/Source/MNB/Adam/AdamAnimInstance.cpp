// Fill out your copyright notice in the Description page of Project Settings.

#include "AdamAnimInstance.h"
#include "Adam/AdamCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAdamAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	AAdamCharacter* Character = Cast<AAdamCharacter>(TryGetPawnOwner());
	if (!Character)
		return;
	
	if (!Character->IsValidLowLevel())
		return;

	Speed = Character->GetCharacterMovement()->Velocity.Size();
	Direction = CalculateDirection(Character->GetCharacterMovement()->Velocity, FRotator(0.0f, Character->GetControlRotation().Yaw, 0.0f));
}
