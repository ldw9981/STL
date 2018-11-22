// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CheckPlayer.h"
#include "Zombie/ZombieCharacter.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "CustomComponents/CharacterStateComponent.h"
EBTNodeResult::Type UBTTask_CheckPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AZombieAIController* AIC = Cast<AZombieAIController>(OwnerComp.GetAIOwner());
	if (ZombieCharacter && AIC)
	{
		AActor* Player = (AActor*)OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ChaseTargetActor"));
		if (Player)
		{
			float Distance = FVector::Distance(ZombieCharacter->GetActorLocation(), Player->GetActorLocation());

			if (Distance > ZombieCharacter->PawnSensing->SightRadius)
			{
//				ZombieCharacter->SetState(EZombieState::Normal);
				ECharacterState NewState = ECharacterState::Normal;
				ZombieCharacter->CharacterState->SetCurrentState(NewState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)NewState);
				return EBTNodeResult::Failed;
			}
			else if (Distance <= ZombieCharacter->AttackRange)
			{
//				ZombieCharacter->SetState(EZombieState::Battle);
				ECharacterState NewState = ECharacterState::Battle;
				ZombieCharacter->CharacterState->SetCurrentState(NewState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)NewState);
				return EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
