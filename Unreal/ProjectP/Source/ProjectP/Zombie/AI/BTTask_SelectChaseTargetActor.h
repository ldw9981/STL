// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SelectChaseTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UBTTask_SelectChaseTargetActor : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
