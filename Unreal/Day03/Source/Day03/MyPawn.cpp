// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
//#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h" //Global Utility
#include "Engine/World.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("몸체"));
	Body->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_P38(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_P38.Succeeded())
	{
		Body->SetStaticMesh(SM_P38.Object);
	}

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("왼쪽"));
	Left->SetupAttachment(Body);
	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("오른쪽"));
	Right->SetupAttachment(Body);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}
	Left->SetRelativeLocation(FVector(37, 21, 0));
	Right->SetRelativeLocation(FVector(37, -21, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(100,0, 0));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotatePropeller(Left, 1080);
	RotatePropeller(Right, 1080);
}

void AMyPawn::RotatePropeller(UStaticMeshComponent* Propeller,float Speed)
{
	Propeller->AddRelativeRotation(FRotator(0, 0, Speed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPawn::Fire);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),  this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPawn::Roll);
	PlayerInputComponent->BindAxis(TEXT("Boost"), this, &AMyPawn::Boost);
}

void AMyPawn::Boost(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyPawn::Fire()
{

}
void AMyPawn::Pitch(float Value)
{
	if (Value != 0.0f)
	{
		AddActorLocalRotation(FRotator(Value, 0, 0));
	}

}

void AMyPawn::Roll(float Value)
{
	if (Value != 0.0f)
	{
		AddActorLocalRotation(FRotator(0, 0, Value));
	}
}