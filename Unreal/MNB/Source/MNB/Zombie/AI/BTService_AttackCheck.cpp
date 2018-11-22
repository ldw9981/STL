// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_AttackCheck.h"
#include "Zombie/ZombieCharacter.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BrainComponent.h"
#include "CustomComponents/CharacterStateComponent.h"

void UBTService_AttackCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogClass, Warning, TEXT("TickNode"));

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
				ECharacterState NewState = ECharacterState::Normal;
				ZombieCharacter->CharacterState->SetCurrentState(NewState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)NewState);
				return;
			}
			else if (Distance <= ZombieCharacter->AttackRange)
			{
				ECharacterState NewState = ECharacterState::Battle;
				ZombieCharacter->CharacterState->SetCurrentState(NewState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)NewState);
				return;
			}
		}
	}
	return;
}
