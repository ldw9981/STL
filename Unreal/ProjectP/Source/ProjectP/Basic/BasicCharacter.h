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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInstance* BulletDecal;


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
	bool bIronSight=false;
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
	float LeanAngle=30.0f;

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

	/**
	 * Apply damage to this actor.
	 * @see https://www.unrealengine.com/blog/damage-in-ue4
	 * @param DamageAmount		How much damage to apply
	 * @param DamageEvent		Data package that fully describes the damage received.
	 * @param EventInstigator	The Controller responsible for the damage.
	 * @param DamageCauser		The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	 * @return					The amount of damage actually applied.
	 */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	bool IsDead();
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

	void DoIronSight();
	void DoJump();

	UFUNCTION()
	void StartFire();
	UFUNCTION()
	void StopFire();
	UFUNCTION()
	void OnFire();
	UFUNCTION()
	void Reload();

	UFUNCTION()
	void StartLeftLean();
	UFUNCTION()
	void StopLeftLean();
	UFUNCTION()
	void StartRightLean();
	UFUNCTION()
	void StopRightLean();

	TArray<class AMasterItem*> PickupItemList;

	void AddPickupItem(class AMasterItem* NewItem);
	void DelPickupItem(class AMasterItem* NewItem);
};
