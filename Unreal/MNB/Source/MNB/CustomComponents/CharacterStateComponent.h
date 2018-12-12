// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Components/ActorComponent.h"
#include "CharacterStateComponent.generated.h"


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	NotSet = 0		UMETA(Display, "NotSet"),
	Normal = 1		UMETA(Display, "Normal"),
	Battle = 2		UMETA(Display, "Battle"),
	Chase = 3		UMETA(Display, "Chase"),
	Dead = 4		UMETA(Display, "Dead")
};

UCLASS(Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MNB_API UCharacterStateComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeStateDelegate,FName, VariableName);
public:	
	// Sets default values for this component's properties
	UCharacterStateComponent();

	/** Delegate to execute when we change State. */
	UPROPERTY(BlueprintAssignable)
	FChangeStateDelegate OnChangeCharacterState;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetCurrentHP, Category = "StateInfo")
	float CurrentHP=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetMaxHP, Category = "StateInfo")
	float MaxHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetCurrentState, Category = "StateInfo")
	ECharacterState CurrentState = ECharacterState::NotSet;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsDead();

	UFUNCTION(BlueprintSetter)
	void SetCurrentHP(float NewCurrentHP);
	float GetCurrentHP();

	UFUNCTION(BlueprintCallable)
	float AddCurrentHP(float AddHP);

	UFUNCTION(BlueprintSetter)
	void SetMaxHP(float NewMaxHP);
	float GetMaxHP();

	UFUNCTION(BlueprintSetter)
	void SetCurrentState(ECharacterState NewCurrentState);
	ECharacterState GetCurrentState();

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	
	UFUNCTION()
	void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintNativeEvent)
	void ReactAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	virtual void ReactAnyDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION(BlueprintNativeEvent)
	void ReactPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	virtual void ReactPointDamage_Implementation(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	
	UFUNCTION(BlueprintNativeEvent)
	void ReactRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);
	virtual void ReactRadialDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);
};
