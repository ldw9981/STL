// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicCharacter.generated.h"

UCLASS()
class PROJECTP_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWeaponComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		bool bIsSprint = false;

	//float WalkSpeed = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float RunSpeed = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float CrouchSpeed = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float SprintSpeed = 600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float CapsuleCrouchHalfHeight = 44.0f;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void Sprint();
	void UnSprint();
	void DoCrouch();
	//  캐릭터 무브먼트에 있음
	//void Crouch();
	void SetSprint();
	void SetRun();
	FRotator GetAimOffset();
};
