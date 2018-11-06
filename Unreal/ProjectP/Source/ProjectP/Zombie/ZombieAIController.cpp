// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"

void AZombieAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	UE_LOG(LogClass, Warning, TEXT("AZombieAIController::Possess"));

//	GetPathFollowingComponent->SetLastMoveAtGoal();
}

void AZombieAIController::UnPossess()
{
	Super::UnPossess();
}
