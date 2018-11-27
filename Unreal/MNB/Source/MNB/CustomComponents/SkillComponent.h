// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

/*
	스킬관련 정보를 갖는다.
*/
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MNB_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
	float SkillDamage = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
	float SkillRange = 100;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
