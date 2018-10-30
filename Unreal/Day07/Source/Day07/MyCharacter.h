// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class DAY07_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Forward(float Value);

	void Right(float Value);

	void Pitch(float Value);

	void Yaw(float Value);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ForwardValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RightValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunningSpeed=100.0f;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BackWalkingSpeed=20.0f;
};
