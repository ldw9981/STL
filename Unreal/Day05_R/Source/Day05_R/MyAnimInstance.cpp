// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if ((Pawn && Pawn->IsValidLowLevel()) == false)
		return;

	
	ForwardValue = Pawn->ForwardValue;
	RightValue = Pawn->RightValue;
	bIsSpeed = Pawn->GetCharacterMovement()->Velocity.Size() > 0 ? true : false;

	float Calculate=0.0f;
	if (Pawn->ForwardValue == 1)
	{
		Calculate = 45 * Pawn->RightValue;
	}
	else if (Pawn->ForwardValue == 0)
	{
		Calculate = 90 * Pawn->RightValue;
	}
	else
	{
		Calculate = -45 * Pawn->RightValue;
	}
	
	if (Calculate != AngleTarget)
	{
		AnglePrev = Angle;
		AngleTarget = Calculate;
		TimePrev = UGameplayStatics::GetTimeSeconds(GetWorld());
	}	

	if (Angle != AngleTarget)
	{
		float TimeCurr = UGameplayStatics::GetTimeSeconds(GetWorld());
		float v = UKismetMathLibrary::FMin((TimeCurr - TimePrev), TimeRotate) / TimeRotate;
		Angle = UKismetMathLibrary::Lerp(AnglePrev, AngleTarget, v);
		//UE_LOG(LogClass, Warning, TEXT("Owner %f %f %f %f"), AnglePrev, AngleTarget, v);
	}	
}
