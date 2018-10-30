// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 연결
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);	
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritRoll = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/Character/Meshes/FuseModel.FuseModel'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->AddLocalOffset(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
		GetMesh()->AddLocalRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("AnimBlueprint'/Game/Blueprints/BP_CharacterAnim.BP_CharacterAnim_C'"));
	if (AnimInstance.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AMyCharacter::Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AMyCharacter::Right);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyCharacter::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
}
void AMyCharacter::Forward(float Value)
{
	ForwardValue = Value;	
	if (Value == 0.0f)
		return;

	FVector InputVector = FRotator(0,Camera->GetComponentRotation().Yaw,0).RotateVector(FVector::ForwardVector);

	float delta = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	if (ForwardValue > 0.0f)
	{
		InputVector *= delta * RunningSpeed * Value;
	}
	else
	{
		InputVector *= delta * BackWalkingSpeed * Value;
	}
	

	AddMovementInput(InputVector);
}
void AMyCharacter::Right(float Value)
{
	RightValue = Value;
	if (Value == 0.0f)
		return;

	FVector InputVector = FRotator(0, Camera->GetComponentRotation().Yaw, 0).RotateVector(FVector::RightVector);

	float delta = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	if (ForwardValue < 0.0f)
	{
		InputVector *= delta * BackWalkingSpeed * Value;
	}
	else
	{
		InputVector *= delta * RunningSpeed * Value;
	}
	AddMovementInput(InputVector);
}
void AMyCharacter::Pitch(float Value)
{
	if (Value == 0.0f)
		return;

	float delta = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddControllerPitchInput(delta * 90.0f * Value);
}
void AMyCharacter::Yaw(float Value)
{
	if (Value == 0.0f)
		return;

	float delta = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddControllerYawInput(delta * 90.0f * Value);
}
