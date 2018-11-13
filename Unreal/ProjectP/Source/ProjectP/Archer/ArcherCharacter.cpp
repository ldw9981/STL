// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcherCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h" //Static(Global) Utiltiy
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstance.h"
#include "Components/DecalComponent.h"

#include "Basic/BasicPlayerCameraManager.h"
#include "Basic/WeaponComponent.h"



// Sets default values
AArcherCharacter::AArcherCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->SetRelativeLocation(FVector(0, 32, 72));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);


	//GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	//Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	//Weapon->SetupAttachment(GetMesh(), TEXT("RHandWeapon"));

	//¾ÉÀ»¶§ ´« ³ôÀÌ °©ÀÚ±â ÀÌµ¿ ¸·±â
	//CapsuleCrouchHalfHeight = GetCharacterMovement()->CrouchedHalfHeight;
	//GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	//NormalSpringPosition = SpringArm->GetRelativeTransform().GetLocation();
	//CrouchSpringPosition = SpringArm->GetRelativeTransform().GetLocation() - FVector(0, 0, 44);

	Tags.Add(TEXT("Player"));

}

// Called when the game starts or when spawned
void AArcherCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	CurrentHP = MaxHP;
}

// Called every frame
void AArcherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArcherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

