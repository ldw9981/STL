// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SelectChaseTargetActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SelectChaseTargetActor::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AActor* Actor = (AActor*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//랜덤 위치 설정
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Actor);
	return EBTNodeResult::Succeeded;
}
