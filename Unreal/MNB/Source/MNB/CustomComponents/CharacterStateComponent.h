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
	Normal = 0		UMETA(Display, "Normal"),
	Battle = 1		UMETA(Display, "Battle"),
	Chase = 2		UMETA(Display, "Chase"),
	Dead = 3		UMETA(Display, "Dead")
};

UCLASS(Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MNB_API UCharacterStateComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeStateDelegate, ECharacterState, NewState );
public:	
	// Sets default values for this component's properties
	UCharacterStateComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MaxHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterState CurrentState = ECharacterState::Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadState")
	bool ChangeRootShapeNoCollision = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadState")
	bool ChangeSkinnedMeshSimulatePhysics = true;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CalculateCurrentHP(float AddHP);
	bool IsDead();

//	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	// C++에서 멤버함수를 구현하고 블루프린트에서 사용하거나 재 정의한다.
	UFUNCTION(BlueprintNativeEvent)
	void ProcessDead();
	virtual void ProcessDead_Implementation();

	UFUNCTION(BlueprintCallable)
	void SetState(ECharacterState NewState);

	UFUNCTION(BlueprintCallable)
	ECharacterState GetState();

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	
	UFUNCTION()
	void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintNativeEvent)
	float CalculateAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	virtual float CalculateAnyDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION(BlueprintNativeEvent)
	float CalculatePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	virtual float CalculatePointDamage_Implementation(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	
	UFUNCTION(BlueprintNativeEvent)
	float CalculateRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);
	virtual float CalculateRadialDamage_Implementation(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	/** Delegate to execute when we change State. */
	UPROPERTY(BlueprintAssignable)
	FChangeStateDelegate EventChangeState;

	// C++에서 멤버함수를 구현하지 않고 블루프인트에서 정의한다. (정의하지않하도 런타임 에러는 발생하지 않는다.)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnFireParticle();
};
