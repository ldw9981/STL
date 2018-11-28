// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_AttackCheck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BrainComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Perception/PawnSensingComponent.h"
#include "CustomComponents/CharacterStateComponent.h"
#include "CustomComponents/SkillComponent.h"

void UBTService_AttackCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));

	APawn* Pawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	AActor* ChaseTargetActor = (AActor*)OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("ChaseTargetActor"));
	if (!Pawn || !AIC || !ChaseTargetActor)
	{
		return;
	}

	UPawnSensingComponent* PawnSensing = Cast<UPawnSensingComponent>(
		Pawn->GetComponentByClass(UPawnSensingComponent::StaticClass()));
	if (!PawnSensing)
	{
		return;
	}

	UCharacterStateComponent* CharacterState = Cast<UCharacterStateComponent>(
		Pawn->GetComponentByClass(UCharacterStateComponent::StaticClass()));
	if (!CharacterState)
	{
		return;
	}

	USkillComponent* Skill = Cast<USkillComponent>(
		Pawn->GetComponentByClass(USkillComponent::StaticClass()));
	if (!Skill)
	{
		return;
	}

	float Distance = FVector::Distance(Pawn->GetActorLocation(), ChaseTargetActor->GetActorLocation());
	if (Distance > PawnSensing->SightRadius)
	{
		ECharacterState NewState = ECharacterState::Normal;
		CharacterState->SetCurrentState(NewState);
	}
	else if (Distance <= Skill->SkillRange)
	{
		ECharacterState NewState = ECharacterState::Battle;
		CharacterState->SetCurrentState(NewState);
	}
}
