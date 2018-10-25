// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class DAY03_R_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Right;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UFloatingPawnMovement* Movement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotateSpeed = 90;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveSpeed = 300;
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Rocket;

	UFUNCTION(BlueprintImplementableEvent)	// C++에서 BP에서 구현된 함수 호출
	void BlueprintTest();
	
	UFUNCTION(BlueprintImplementableEvent)	// 기본구현은 C++ 구현   BP에서 오버라이딩 (재정의가능)
	void BlueprintNative();
	virtual void BlueprintNative_Implementation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RotatePropeller(class UMeshComponent * MeshComponent, float Speed);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Pitch(float Value);

	void Roll(float Value);

	void Boost(float Value);

	void Fire();
};
