// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"
#include "Animation/AnimationAsset.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	USkeletalMeshComponent* SM = GetMesh();
	SM->AddLocalOffset(FVector(0.0f,0.0f,-GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	SM->AddLocalRotation(FRotator(0.0f,-90.0f,0.0f));	

	/*
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("AnimBlueprint'/Game/Blueprints/BP_MyCharacterAnim.BP_MyCharacterAnim_C'"));	
	if (AnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}
	*/
	
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
	PlayerInputComponent->BindAxis(TEXT("Forward"),this,&AMyCharacter::Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AMyCharacter::Right);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyCharacter::Pitch);	
}

void AMyCharacter::Forward(float Value)
{
	ForwardValue = Value;
	if (Value == 0.0f)
		return;

	/*
	// Roll축 벡터를 카메라 Yaw회전량 만큼 회전 
	FRotator rotator = Camera->GetComponentRotation();
	rotator.Pitch = 0.0f;
	rotator.Roll = 0.0f;
	FVector vector = rotator.RotateVector(FVector(1.0f, 0.0f, 0.0f));
	AddMovementInput(vector, Value);
	*/
	float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	if (Value > 0.0f)
	{
		Value *= deltaTime * ForwardSpeed;
	}
	else
	{
		Value *= deltaTime * BackSpeed;
	}

	

	FRotator ViewRotation = FRotator(0,Camera->GetComponentRotation().Yaw,0);
//	FRotator ViewRotation = FRotator(0,GetControlRotation().Yaw, 0);
	FVector InputVector = UKismetMathLibrary::GetForwardVector(ViewRotation);
	AddMovementInput(InputVector, Value);
}

void AMyCharacter::Right(float Value)
{
	RightValue = Value;
	if (Value == 0.0f)
		return;

	float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	if (ForwardValue >= 0.0f)
	{
		Value *= deltaTime * ForwardSpeed;
	}
	else
	{
		Value *= deltaTime * BackSpeed;
	}
	
	/*
	// Pitch축 벡터를 카메라 Yaw회전량 만큼 회전 
	FRotator rotator = Camera->GetComponentRotation();
	rotator.Pitch = 0.0f;
	rotator.Roll = 0.0f;
	FVector vector = rotator.RotateVector(FVector(0.0f, 1.0f, 0.0f));
	AddMovementInput(vector, Value);
	*/
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, Camera->GetComponentRotation().Yaw, 0)), Value);
}

void AMyCharacter::Yaw(float Value)
{
	if (Value == 0.0f)
		return;

	// 컨트롤러에 회전값 전달
	AddControllerYawInput(Value);
}
void AMyCharacter::Pitch(float Value)
{
	if (Value == 0.0f)
		return;

	// 컨트롤러에 회전값 전달
	AddControllerPitchInput(Value);
}