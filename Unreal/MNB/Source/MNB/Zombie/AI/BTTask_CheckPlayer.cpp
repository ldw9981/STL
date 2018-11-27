// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CheckPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BrainComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Perception/PawnSensingComponent.h"
#include "CustomComponents/CharacterStateComponent.h"
#include "CustomComponents/SkillComponent.h"

EBTNodeResult::Type UBTTask_CheckPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	APawn* Pawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	AActor* ChaseTargetActor = (AActor*)OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ChaseTargetActor"));
	if (!Pawn || !AIC || !ChaseTargetActor)
	{
		return EBTNodeResult::Failed;
	}

	TArray <UPawnSensingComponent *> PawnSensingComponents;
	Pawn->GetComponents(PawnSensingComponents);
	if (PawnSensingComponents.Num() != 1)
	{
		return EBTNodeResult::Failed;
	}

	TArray <UCharacterStateComponent *> CharacterStateComponents;
	Pawn->GetComponents(CharacterStateComponents);
	if (CharacterStateComponents.Num() != 1)
	{
		return EBTNodeResult::Failed;
	}

	TArray <USkillComponent *> SkillComponents;
	Pawn->GetComponents(SkillComponents);
	if (SkillComponents.Num() != 1)
	{
		return EBTNodeResult::Failed;
	}
	float Distance = FVector::Distance(Pawn->GetActorLocation(), ChaseTargetActor->GetActorLocation());
	if (Distance > PawnSensingComponents[0]->SightRadius)
	{
		ECharacterState NewState = ECharacterState::Normal;
		CharacterStateComponents[0]->SetCurrentState(NewState);
		return EBTNodeResult::Failed;
	}
	else if (Distance <= SkillComponents[0]->SkillRange)
	{
		ECharacterState NewState = ECharacterState::Battle;
		CharacterStateComponents[0]->SetCurrentState(NewState);
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Succeeded;
}
