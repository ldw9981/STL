// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h" //GetWorld
#include "Kismet/GameplayStatics.h" //Static(Global) Utiltiy
#include "Components/InputComponent.h"
#include "MyRocket.h"



// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);

	Left = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Right = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	Arrow->SetRelativeLocation(FVector(100, 0, 0));

	Right->SetRelativeLocation(FVector(37, 21, 0));
	Left->SetRelativeLocation(FVector(37, -21, 0));

	Movement->MaxSpeed = MoveSpeed;

	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	Box->SetBoxExtent(FVector(29, 8, 8));
//	Box->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPawn::RotatePropeller(UStaticMeshComponent* Propeller, float Speed)
{
	Propeller->AddLocalRotation(FRotator(0, 0, Speed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotatePropeller(Left, RotateSpeed);
	RotatePropeller(Right, RotateSpeed);
}



// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Boost"), this, &AMyPawn::Boost);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPawn::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPawn::Fire);

}

void AMyPawn::Boost(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyPawn::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddActorLocalRotation(FRotator(60 * Value * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
			0,
			0)
		);
	}
}

void AMyPawn::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddActorLocalRotation(FRotator(0, 0,
			60 * Value * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()))
		);
	}

}

void AMyPawn::Fire()
{
//	FActorSpawnParameters F;
	
	GetWorld()->SpawnActor<AMyRocket>(Arrow->GetComponentLocation(),
		Arrow->GetComponentRotation());
}

void AMyPawn::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
}
