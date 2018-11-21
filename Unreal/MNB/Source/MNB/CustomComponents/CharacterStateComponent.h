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
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	// C++���� ����Լ��� �����ϰ� �������Ʈ���� ����ϰų� �� �����Ѵ�.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float CalculateHP();
	virtual float CalculateHP_Implementation();

	virtual void OnChangeState(ECharacterState NewState);
	UFUNCTION(BlueprintCallable)
	void SetState(ECharacterState NewState);
	UFUNCTION(BlueprintCallable)
	ECharacterState GetState();
	bool IsDead();

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	UFUNCTION()
	void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);
public:
	/** Delegate to execute when we change State. */
	UPROPERTY(BlueprintAssignable)
	FChangeStateDelegate EventChangeState;

	// C++���� ����Լ��� �������� �ʰ� �������Ʈ���� �����Ѵ�. (�����������ϵ� ��Ÿ�� ������ �߻����� �ʴ´�.)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnFireParticle();
};
