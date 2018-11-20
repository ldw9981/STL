// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStateComponent.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"

// Sets default values for this component's properties
UCharacterStateComponent::UCharacterStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHP = MaxHP;
}


// Called every frame
void UCharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UCharacterStateComponent::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentHP <= 0)
		return 0;

	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))	//범위 데미지
	{
		UE_LOG(LogClass, Warning, TEXT("TakeDamage FRadialDamageEvent: %f"), DamageAmount);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))	//점 데미지
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
		UE_LOG(LogClass, Warning, TEXT("TakeDamage FPointDamageEvent: %f %s"), DamageAmount, *(PointDamageEvent->HitInfo.BoneName.ToString()));


		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;

		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))			// 일반데미지
	{
		UE_LOG(LogClass, Warning, TEXT("TakeDamage FDamageEvent: %f"), DamageAmount);
		CurrentHP -= DamageAmount;
	}

	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		SetState(ECharacterState::Dead);
	}

	return DamageAmount;
}

void UCharacterStateComponent::OnStateChanged(ECharacterState NewState)
{
	if (NewState == ECharacterState::Dead)
	{
		// 지나가는데 방해되지않게 컬리전 비활성
		UShapeComponent* RootCollision = Cast<UShapeComponent>(GetOwner()->GetRootComponent());
		if (RootCollision)
		{
			RootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

inline void UCharacterStateComponent::SetState(ECharacterState NewState)
{
	if (NewState != CurrentState)
	{
		CurrentState = NewState;
		OnStateChanged(CurrentState);	// 추가 작업은 이곳에 맡긴다.
	}
}

inline bool UCharacterStateComponent::IsDead()
{
	if (CurrentHP <= 0)
		return true;

	return false;
}

