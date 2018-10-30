// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"
#include "MyCharacter.h"

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if ((MyCharacter && MyCharacter->IsValidLowLevel()) == false)
		return;

	ForwardValue = MyCharacter->ForwardValue;
	RightValue = MyCharacter->RightValue;
	ExistSpeed = MyCharacter->GetVelocity().Size() > 0 ? true : false;

	if (ForwardValue == 1.0f)
	{
		Angle = RightValue * 45.0f;
	}
	else if (ForwardValue == 0.0f)
	{
		Angle = 90.0f * RightValue;
	}
	else
	{
		Angle = RightValue * -45.0f;
	}

}
