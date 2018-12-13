// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/ItemDataTable.h"
#include "BasicCharacter.generated.h"

UCLASS()
class PROJECTBD_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetHPBar();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CheckItem();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Reload();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWeaponComponent* Weapon;


	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);

	void Sprint();
	void UnSprint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State", Replicated)
	bool bIsSprint = false;

	//float WalkSpeed = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float SprintSpeed = 700;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float RunSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CrouchSpeed = 150.0f;

	void DoCrouch();
	void DoIronsight();

	FRotator GetAimOffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CapsuleCrouchHalfHeight = 44.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	bool bIsIronsight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector NormalSpringPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector CrouchSpringPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Replicated)
	bool bIsFire = false;

	UFUNCTION()
	void StartFire();

	UFUNCTION()
	void StopFire();

	UFUNCTION()
	void OnFire();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class USoundBase* FireSound;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAcess = true))
	class UMaterialInstance* BulletDecal;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State", Replicated)
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Replicated)
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* DeadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsReload = false;

	UFUNCTION()
	void StartLeftLean();

	UFUNCTION()
	void StopLeftLean();

	UFUNCTION()
	void StartRightLean();

	UFUNCTION()
	void StopRightLean();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bLeftLean = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bRightLean = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float LeanAngle = 30.0f;

	bool IsDead();

	TArray<class AMasterItem*> PickupItemList;

	void AddPickupItem(class AMasterItem* NewItem);
	void RemovePickupItem(class AMasterItem* NewItem);
	int GetClosestItem(FVector SightLocation);
	FVector GetSightLocation();

	FTimerHandle ItemCheckHandle;

	void Use();

	void ToggleInventory();

	void DropItem(FItemDataTable ItemData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TSubclassOf<class AMasterItem> MasterItem;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_DoIronsight();
	bool C2S_DoIronsight_Validate();
	void C2S_DoIronsight_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SetSprint(bool Sprint);
	bool C2S_SetSprint_Validate(bool Sprint);
	void C2S_SetSprint_Implementation(bool Sprint);

};
