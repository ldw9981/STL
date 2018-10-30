// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		ForwardValue = Pawn->ForwardValue;
		RightValue = Pawn->RightValue;

		bIsSpeed = Pawn->GetCharacterMovement()->Velocity.Size() > 0 ? true : false;

		//Forward 1
			//Right 1
			//Right 0
			//Right -1
			//45, 
		if (Pawn->ForwardValue == 1)
		{
			Angle = 45 * Pawn->RightValue;
		}
		//Forward 0
			//Right 1
			//Right 0
			//Right -1
			//90
		else if (Pawn->ForwardValue == 0)
		{
			Angle = 90 * Pawn->RightValue;
		}
		//Forward -1
			//Right 1
			//Right 0
			//Right -1
			//-45
		else
		{
			Angle = -45 * Pawn->RightValue;
		}
	}
}
