// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_PlayerAttack.h"
#include "Zombie/ZombieCharacter.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// 이 노티는 테스트용으로 좀비가 때리는 순간 벗어나 있어도 데미지가 들어온다.
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));

	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(MeshComp->GetOwner());
	if (!ZombieCharacter)
	{	// 엔진에서 애니메이션 창에서 실행하면 에디터
		return;
	}
		

	AZombieAIController* AIC = Cast<AZombieAIController>(ZombieCharacter->GetController());
	if (!AIC)
	{
		return;
	}

	AActor* Player = Cast<AActor>(AIC->BBComponent->GetValueAsObject(FName("ChaseTargetActor")));
	if (!Player)
	{
		return;
	}	

	UGameplayStatics::ApplyDamage(Player, 30.0f, AIC, ZombieCharacter, nullptr);
}
