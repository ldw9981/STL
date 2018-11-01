// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Basic/WeaponComponent.h"
#include "Components/CapsuleComponent.h"
// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0,-GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->SetRelativeLocation(FVector(0, 32, 72));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(),TEXT("RHandWeapon"));

	//앉을때 카메라 높이 변경 막기
	CapsuleCrouchHalfHeight = GetCharacterMovement()->CrouchedHalfHeight;
	GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();	
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABasicCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABasicCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABasicCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABasicCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ABasicCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ABasicCharacter::UnSprint);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ABasicCharacter::DoCrouch);
}
void ABasicCharacter::MoveForward(float Value)
{
	if (Value == 0.0f)
		return;

	if (Value == -1.0f && bIsSprint)
	{
		SetRun();
	}

	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)), Value);

	// AddMovementInput(GetActorForward(),Value);
}
void ABasicCharacter::MoveRight(float Value)
{
	if (Value == 0.0f)
		return;

	if (bIsSprint)
	{
		SetRun();
	}
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)), Value);
	// AddMovementInput(GetActorRight(),Value);
}
void ABasicCharacter::LookUp(float Value)
{
	if (Value == 0.0f)
		return;

	AddControllerPitchInput(Value);
}
void ABasicCharacter::Turn(float Value)
{
	if (Value == 0.0f)
		return;

	AddControllerYawInput(Value);
}

void ABasicCharacter::Sprint()
{
	SetSprint();
}

void ABasicCharacter::UnSprint()
{
	SetRun();
}
void ABasicCharacter::DoCrouch()
{
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ABasicCharacter::SetSprint()
{
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABasicCharacter::SetRun()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

FRotator ABasicCharacter::GetAimOffset()
{
	/*
	FVector AimDirWS = GetBaseAimRotation().Vector();
	FVector AimDirLS = ActorToWorld().InverseTransformPositionNoScale(AimDirWS);
	FRotator AimRotLS = AimDirLS.Rotation();
	return AimRotLS;
	*/
	return ActorToWorld().InverseTransformPositionNoScale(GetBaseAimRotation().Vector()).Rotation();
}