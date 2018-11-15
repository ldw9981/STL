// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_AttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class MNB_API UBTService_AttackCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	/** update next tick interval
		 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
