// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArcherCharacter.generated.h"

UCLASS()
class PROJECTP_API AArcherCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArcherCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool bIsSprint = false;
	//float WalkSpeed = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float RunSpeed = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float CrouchSpeed = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float SprintSpeed = 700;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float CapsuleCrouchHalfHeight = 44.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool bIronSight = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		FVector NormalSpringPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		FVector CrouchSpringPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool bIsFire = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool bLeftLean = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool bRightLean = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float LeanAngle = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float MaxHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool bIsReload = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* DeadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* ReloadAnimation;
	float CurrentZ;
	FVector NormalSpringArm;
	FVector CurrentSpringArm;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
