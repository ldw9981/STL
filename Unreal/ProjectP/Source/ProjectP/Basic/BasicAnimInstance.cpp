// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicAnimInstance.h"
#include "Basic/BasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponComponent.h"

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
		bIsReload = Pawn->bIsReload;
		bIsFire = Pawn->bIsFire;
		bLeftLean = Pawn->bLeftLean;
		bRightLean = Pawn->bRightLean;

		float TargetAngle = 0.0f;
		if (bIsReload)
		{
			if (!Montage_IsPlaying(Pawn->ReloadAnimation))
			{
				// 몽타주이벤트 테스트
				//OnMontageBlendingOut.AddDynamic
				Montage_Play(Pawn->ReloadAnimation);
			}
		}

		
		if (bLeftLean)
		{
			TargetAngle = -Pawn->LeanAngle;
		}
		else if (bRightLean)
		{
			TargetAngle = Pawn->LeanAngle;
		}
		else if (bLeftLean && bRightLean)
		{
			TargetAngle = 0;
		}

		CurrentAngle = FMath::FInterpTo(CurrentAngle, TargetAngle, DeltaSeconds, 20.0f);

		//UE_LOG(LogClass, Warning, TEXT("%d %d JumpVelocityZ %f"), bIsFalling, Pawn->GetCharacterMovement()->IsFlying(), JumpVelocityZ);
	}
}

void UBasicAnimInstance::AnimNotify_ReloadComplete(UAnimNotify * Notify)
{
	ABasicCharacter* Pawn = Cast<ABasicCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->Weapon->ReloadComplete();
		bIsReload = false;
		Pawn->bIsReload = false;
	}
}
