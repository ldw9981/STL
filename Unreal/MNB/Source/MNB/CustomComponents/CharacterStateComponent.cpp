// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStateComponent.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"
#include "Components/SkinnedMeshComponent.h"
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
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UCharacterStateComponent::OnTakeAnyDamage);
	GetOwner()->OnTakePointDamage.AddDynamic(this, &UCharacterStateComponent::OnTakePointDamage);
	GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UCharacterStateComponent::OnTakeRadialDamage);
}


// Called every frame
void UCharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/*
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
*/


float UCharacterStateComponent::CalculateHP_Implementation()
{
	UE_LOG(LogClass, Warning, TEXT("C++ CalculateHP_Implementation"));
	return 1.0f;
}

void UCharacterStateComponent::OnChangeState(ECharacterState NewState)
{
	
	if (NewState == ECharacterState::Dead)
	{
		// 지나가는데 방해되지않게 컬리전 비활성
		UShapeComponent* RootCollision = Cast<UShapeComponent>(GetOwner()->GetRootComponent());
		if (RootCollision)
		{
			RootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		TArray <USkinnedMeshComponent *> Container;
		GetOwner()->GetComponents(Container);
		if (Container.Num() > 0)
		{
			for (auto SkinnedMesh : Container)
			{				
				SkinnedMesh->SetSimulatePhysics(true);				
			}
		}

	}
}

inline void UCharacterStateComponent::SetState(ECharacterState NewState)
{
	if (NewState != CurrentState)
	{
		CurrentState = NewState;
		EventChangeState.Broadcast(NewState);
		OnChangeState(CurrentState);	// 추가 작업은 이곳에 맡긴다.
		CalculateHP();
	}
}

ECharacterState UCharacterStateComponent::GetState()
{
	return CurrentState;
}

inline bool UCharacterStateComponent::IsDead()
{
	if (CurrentHP <= 0)
		return true;

	return false;
}

void UCharacterStateComponent::OnTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	UE_LOG(LogClass, Warning, TEXT("TakeDamage OnTakeAnyDamage: %f"), Damage);
	CurrentHP -= Damage;
	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		SetState(ECharacterState::Dead);
	}
}

void UCharacterStateComponent::OnTakePointDamage(AActor * DamagedActor, float Damage, AController * InstigatedBy, FVector HitLocation, UPrimitiveComponent * FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType * DamageType, AActor * DamageCauser)
{
	UE_LOG(LogClass, Warning, TEXT("TakeDamage OnTakePointDamage: %f"), Damage);
	CurrentHP -= Damage;
	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		SetState(ECharacterState::Dead);
	}
}

void UCharacterStateComponent::OnTakeRadialDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, FVector Origin, FHitResult HitInfo, AController * InstigatedBy, AActor * DamageCauser)
{
	UE_LOG(LogClass, Warning, TEXT("TakeDamage OnTakeRadialDamage: %f"), Damage);
	CurrentHP -= Damage;
	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		SetState(ECharacterState::Dead);
	}
}
/*
void UCharacterStateComponent::AddCurrentHP(float AddHP)
{
	CurrentHP += AddHP;
	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		SetState(ECharacterState::Dead);
	}

}

void UCharacterStateComponent::OnChangeCurrentHP()
{
}
*/