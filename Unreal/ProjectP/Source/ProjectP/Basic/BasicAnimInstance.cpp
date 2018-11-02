// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicAnimInstance.h"
#include "Basic/BasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBasicAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABasicCharacter* Pawn = Cast<ABasicCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
		Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity, FRotator(0.0f,Pawn->GetControlRotation().Yaw,0.0f));
		bIsSprint = Pawn->bIsSprint;
		bIsCrouched = Pawn->bIsCrouched;


		FRotator AimRotator = Pawn->GetAimOffset();
		AimPitch = AimRotator.Pitch;
		AimYaw = AimRotator.Yaw;
		bIronSight = Pawn->bIronSight;
		bIsFalling = Pawn->GetCharacterMovement()->IsFalling();
		JumpVelocityZ = Pawn->GetCharacterMovement()->Velocity.Z;

		//UE_LOG(LogClass, Warning, TEXT("%d %d JumpVelocityZ %f"), bIsFalling, Pawn->GetCharacterMovement()->IsFlying(), JumpVelocityZ);
	}
}
