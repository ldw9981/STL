// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPlayerCameraManager.h"
#include "Basic/BasicCharacter.h"
#include "Basic/BasicPC.h"
#include "GameFramework/SpringArmComponent.h"
ABasicPlayerCameraManager::ABasicPlayerCameraManager()
{

}

void ABasicPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	ABasicCharacter* Pawn = Cast<ABasicCharacter>(GetOwningPlayerController()->GetPawn());
	if ((Pawn && Pawn->IsValidLowLevel()) == false)
		return;

	FVector TargetPosition = Pawn->bIsCrouched ? Pawn->CrouchSpringPosition : Pawn->NormalSpringPosition;
	CurrentSpringArm = FMath::VInterpTo(CurrentSpringArm, TargetPosition, DeltaTime, 15.0f);
	Pawn->SpringArm->SetRelativeLocation(CurrentSpringArm);
	UE_LOG(LogClass, Warning, TEXT("%f %f %f"), CurrentSpringArm.X, CurrentSpringArm.Y, CurrentSpringArm.Z);

	float TargetFOV = Pawn->bIronSight ? IronSightFOV : NormalFOV;
	DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 15.0f);
	SetFOV(DefaultFOV);
	// FOV가 사용되므로 후 업데이트 사용
	Super::UpdateCamera(DeltaTime);
}
