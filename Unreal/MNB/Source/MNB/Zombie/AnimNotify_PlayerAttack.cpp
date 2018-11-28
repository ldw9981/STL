// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_PlayerAttack.h"
#include "Zombie/ZombieCharacter.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UAnimNotify_PlayerAttack::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// �� ��Ƽ�� �׽�Ʈ������ ���� ������ ���� ��� �־ �������� ���´�.
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character)
	{	// �������� �ִϸ��̼� â���� �����ϸ� ������
		return;
	}		

	AAIController* AIC = Cast<AAIController>(Character->GetController());
	if (!AIC)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = AIC->GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return;
	}

	AActor* ChaseTargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(FName("ChaseTargetActor")));
	if (!ChaseTargetActor)
	{
		return;
	}	

	UGameplayStatics::ApplyDamage(ChaseTargetActor, 30.0f, AIC, Character, nullptr);
}
