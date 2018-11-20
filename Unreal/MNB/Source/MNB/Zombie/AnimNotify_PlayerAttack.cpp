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
	// �� ��Ƽ�� �׽�Ʈ������ ���� ������ ���� ��� �־ �������� ���´�.
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));

	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(MeshComp->GetOwner());
	if (!ZombieCharacter)
	{	// �������� �ִϸ��̼� â���� �����ϸ� ������
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
