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
	SetCurrentState(ECharacterState::Normal);
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


inline void UCharacterStateComponent::SetCurrentState(ECharacterState NewCurrentState)
{
	if (NewCurrentState != CurrentState)
	{
		CurrentState = NewCurrentState;
		OnChangeCharacterState.Broadcast(TEXT("CurrentState"));
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
	UE_LOG(LogClass, Warning, TEXT("DamagedActor %s TakeDamage OnTakeAnyDamage: %f -> %f" ), *DamagedActor->GetName(), Damage,Result);
	CalculateCurrentHP(-Result);
}

void UCharacterStateComponent::OnTakePointDamage(AActor * DamagedActor, float Damage, AController * InstigatedBy, FVector HitLocation, UPrimitiveComponent * FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType * DamageType, AActor * DamageCauser)
{
	if (IsDead())
	{
		return;
	}

	float Result = CalculatePointDamage(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
	UE_LOG(LogClass, Warning, TEXT("DamagedActor %s TakeDamage OnTakePointDamage: %s %f -> %f"), *DamagedActor->GetName(), *BoneName.ToString(), Damage, Result);
	CalculateCurrentHP(-Result);
}

void UCharacterStateComponent::OnTakeRadialDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, FVector Origin, FHitResult HitInfo, AController * InstigatedBy, AActor * DamageCauser)
{
	if (IsDead())
	{
		return;
	}

	float Result = CalculateRadialDamage(DamagedActor, Damage, DamageType, Origin, HitInfo, InstigatedBy, DamageCauser);
	UE_LOG(LogClass, Warning, TEXT("DamagedActor %s TakeDamage OnTakeRadialDamage: %f -> %f"), *DamagedActor->GetName(), Damage, Result);
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
		UE_LOG(LogClass, Warning, TEXT("Dead"));
	}
}

float UCharacterStateComponent::CalculateAnyDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	return Damage;
}

float UCharacterStateComponent::CalculatePointDamage_Implementation(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	return Damage;
}


float UCharacterStateComponent::CalculateRadialDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	return Damage;
}