// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_IsDead.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "GameFramework/Actor.h"
#include "CustomComponents/CharacterStateComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_IsDead::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	UCharacterStateComponent* TargetCharacterState = Cast<UCharacterStateComponent>(
		Target->GetComponentByClass(UCharacterStateComponent::StaticClass()));
	if (!TargetCharacterState)
	{
		return EBTNodeResult::Failed;
	}

	if (!TargetCharacterState->IsDead())
	{
		return EBTNodeResult::Failed;
	}

	APawn* PawnAI = OwnerComp.GetAIOwner()->GetPawn();
	if (PawnAI)
	{
		UCharacterStateComponent* PawnAICharacterState = Cast<UCharacterStateComponent>(
			PawnAI->GetComponentByClass(UCharacterStateComponent::StaticClass()));
		if (PawnAICharacterState)
		{
			PawnAICharacterState->SetCurrentState(ECharacterState::Normal);
		}	
	}		
	return EBTNodeResult::Succeeded;
}
