// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_IsDead.h"
#include "Adam/AdamCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie/ZombieCharacter.h"
#include "BrainComponent.h"
#include "Zombie/ZombieAIController.h"
#include "CharacterState/CharacterStateComponent.h"

EBTNodeResult::Type UBTTask_IsDead::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AAdamCharacter* Player = Cast<AAdamCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (Player && Player->CharacterState->IsDead())
	{
		AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
		if (ZombieCharacter)
		{
			//ZombieCharacter->SetState(EZombieState::Normal);
			ECharacterState NewState = ECharacterState::Normal;
			ZombieCharacter->CharacterState->SetState(NewState);
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)NewState);
			return EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Succeeded;
}
