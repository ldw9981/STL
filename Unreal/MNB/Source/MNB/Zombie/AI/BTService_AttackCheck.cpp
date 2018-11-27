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

	TArray <UPawnSensingComponent *> PawnSensingComponents;
	Pawn->GetComponents(PawnSensingComponents);
	if (PawnSensingComponents.Num() != 1)
	{
		return;
	}

	TArray <UCharacterStateComponent *> CharacterStateComponents;
	Pawn->GetComponents(CharacterStateComponents);
	if (CharacterStateComponents.Num() != 1)
	{
		return;
	}

	TArray <USkillComponent *> SkillComponents;
	Pawn->GetComponents(SkillComponents);
	if (SkillComponents.Num() != 1)
	{
		return;
	}

	float Distance = FVector::Distance(Pawn->GetActorLocation(), ChaseTargetActor->GetActorLocation());
	if (Distance > PawnSensingComponents[0]->SightRadius)
	{
		ECharacterState NewState = ECharacterState::Normal;
		CharacterStateComponents[0]->SetCurrentState(NewState);		
	}
	else if (Distance <= SkillComponents[0]->SkillRange)
	{
		ECharacterState NewState = ECharacterState::Battle;
		CharacterStateComponents[0]->SetCurrentState(NewState);		
	}
}
