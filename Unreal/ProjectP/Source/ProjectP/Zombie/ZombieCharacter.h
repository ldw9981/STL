// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieCharacter.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Normal	= 0		UMETA(Display, "Normal"),
	Battle	= 1		UMETA(Display, "Battle"),
	Chase	= 2		UMETA(Display, "Chase"),
	Dead	= 3		UMETA(Display, "Dead")
};

UCLASS()
class PROJECTP_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MaxHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float WalkSpeed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float RunSpeed = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float Attack = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float AttackRange = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EZombieState CurrentState = EZombieState::Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTree;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPawnSensingComponent* PawnSensing;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
	UFUNCTION()
	void OnHearNoise(APawn* Pawn,const FVector& Location,float Volume);

	bool SetSpeed(EZombieState NewState);
	void SetState(EZombieState NewState);
};
