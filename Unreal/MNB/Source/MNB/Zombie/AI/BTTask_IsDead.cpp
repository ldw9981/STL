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
	AActor* ActorTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!ActorTarget)
	{
		return EBTNodeResult::Failed;
	}

	TArray <UCharacterStateComponent *> Container;
	ActorTarget->GetComponents(Container);
	if (Container.Num() != 1)
	{
		return EBTNodeResult::Failed;
	}

	if (!Container[0]->IsDead())
	{
		return EBTNodeResult::Failed;
	}

	APawn* PawnAI = OwnerComp.GetAIOwner()->GetPawn();
	if (PawnAI)
	{
		TArray <UCharacterStateComponent *> CharacterStateComponentsAI;
		PawnAI->GetComponents(CharacterStateComponentsAI);
		if (CharacterStateComponentsAI.Num() == 1)
		{
			CharacterStateComponentsAI[0]->SetCurrentState(ECharacterState::Normal);
		}		
	}		
	return EBTNodeResult::Succeeded;
}
