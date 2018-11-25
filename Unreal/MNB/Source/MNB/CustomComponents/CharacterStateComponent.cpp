// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterStateComponent.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "CustomDamageType/CustomDamageType.h"
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
	SetCurrentHP(MaxHP);
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UCharacterStateComponent::OnTakeAnyDamage);
	GetOwner()->OnTakePointDamage.AddDynamic(this, &UCharacterStateComponent::OnTakePointDamage);
	GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UCharacterStateComponent::OnTakeRadialDamage);
}

void UCharacterStateComponent::SetCurrentHP(float NewCurrentHP)
{
	if (CurrentHP != NewCurrentHP)
	{
		CurrentHP = NewCurrentHP;		
		OnChangeCharacterState.Broadcast(TEXT("CurrentHP"));
	}
}

float UCharacterStateComponent::GetCurrentHP()
{
	return CurrentHP;
}

void UCharacterStateComponent::SetMaxHP(float NewMaxHP)
{
	if (MaxHP != NewMaxHP)
	{
		MaxHP = NewMaxHP;
		OnChangeCharacterState.Broadcast(TEXT("MaxHP"));
	}

	UPropertyHelpers.
}

float UCharacterStateComponent::GetMaxHP()
{
	return MaxHP;
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


void UCharacterStateComponent::ProcessDead_Implementation()
{
	if (ChangeRootShapeNoCollision)
	{
		// 지나가는데 방해되지않게 컬리전 비활성
		UShapeComponent* RootCollision = Cast<UShapeComponent>(GetOwner()->GetRootComponent());
		if (RootCollision)
		{
			RootCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	if (ChangeSkinnedMeshSimulatePhysics)
	{
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

inline void UCharacterStateComponent::SetCurrentState(ECharacterState NewCurrentState)
{
	if (NewCurrentState != CurrentState)
	{
		CurrentState = NewCurrentState;
		OnChangeCharacterState.Broadcast(TEXT("CurrentState"));
		if (NewCurrentState == ECharacterState::Dead)
		{
			ProcessDead();
		}
	}	
}

ECharacterState UCharacterStateComponent::GetCurrentState()
{
	return CurrentState;
}

inline bool UCharacterStateComponent::IsDead()
{
	if (CurrentHP <= 0 || CurrentState == ECharacterState::Dead)
		return true;

	return false;
}

void UCharacterStateComponent::OnTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{	
	const UCustomDamageType* CustomDamageType = Cast<UCustomDamageType>(DamageType);
	if (CustomDamageType != nullptr && CustomDamageType->CustomDamageEventType != ECustomDamageEventType::Generic)
	{
		return;
	}		

	if (IsDead())
	{
		return;
	}
		

	float Result = CalculateAnyDamage(DamagedActor, Damage, DamageType,InstigatedBy, DamageCauser);
	UE_LOG(LogClass, Warning, TEXT("TakeDamage OnTakeAnyDamage: %f -> %f" ), Damage,Result);
	CalculateCurrentHP(-Result);
}

void UCharacterStateComponent::OnTakePointDamage(AActor * DamagedActor, float Damage, AController * InstigatedBy, FVector HitLocation, UPrimitiveComponent * FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType * DamageType, AActor * DamageCauser)
{
	if (IsDead())
	{
		return;
	}

	float Result = CalculatePointDamage(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
	UE_LOG(LogClass, Warning, TEXT("TakeDamage OnTakePointDamage: %s %f -> %f"), *BoneName.ToString(), Damage, Result);
	CalculateCurrentHP(-Result);
}

void UCharacterStateComponent::OnTakeRadialDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, FVector Origin, FHitResult HitInfo, AController * InstigatedBy, AActor * DamageCauser)
{
	if (IsDead())
	{
		return;
	}

	float Result = CalculateRadialDamage(DamagedActor, Damage, DamageType, Origin, HitInfo, InstigatedBy, DamageCauser);
	UE_LOG(LogClass, Warning, TEXT("TakeDamage OnTakeRadialDamage: %f -> %f"), Damage, Result);
	CalculateCurrentHP(-Result);
}

void UCharacterStateComponent::CalculateCurrentHP(float AddHP)
{
	if (IsDead())
	{
		return;
	}

	float NewCurrentHP = CurrentHP + AddHP;
	if (NewCurrentHP <= 0)
	{
		NewCurrentHP = 0;
	}
	SetCurrentHP(NewCurrentHP);
	if (NewCurrentHP == 0)
	{
		SetCurrentState(ECharacterState::Dead);
	}
}

float UCharacterStateComponent::CalculateAnyDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	return Damage;
}

float UCharacterStateComponent::CalculatePointDamage_Implementation(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	if (BoneName.Compare(TEXT("head")))
	{
		Damage = Damage * 1.5f;
	}

	return Damage;
}


float UCharacterStateComponent::CalculateRadialDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	return Damage;
}