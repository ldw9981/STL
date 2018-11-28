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

	UPawnSensingComponent* PawnSensing = Cast<UPawnSensingComponent>(
		Pawn->GetComponentByClass(UPawnSensingComponent::StaticClass()));
	if (!PawnSensing)
	{
		return EBTNodeResult::Failed;
	}


	UCharacterStateComponent* CharacterState = Cast<UCharacterStateComponent>(
		Pawn->GetComponentByClass(UCharacterStateComponent::StaticClass()));
	if (!CharacterState)
	{
		return EBTNodeResult::Failed;
	}

	USkillComponent* Skill = Cast<USkillComponent>(
		Pawn->GetComponentByClass(USkillComponent::StaticClass()));
	if (!Skill)
	{
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Distance(Pawn->GetActorLocation(), ChaseTargetActor->GetActorLocation());
	if (Distance > PawnSensing->SightRadius)
	{
		ECharacterState NewState = ECharacterState::Normal;
		CharacterState->SetCurrentState(NewState);
		return EBTNodeResult::Failed;
	}
	else if (Distance <= Skill->SkillRange)
	{
		ECharacterState NewState = ECharacterState::Battle;
		CharacterState->SetCurrentState(NewState);
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Succeeded;
}
