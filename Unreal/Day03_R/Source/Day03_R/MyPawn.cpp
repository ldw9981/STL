// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "ConstructorHelpers.h" //생성자에서 사용
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MyRocket.h"
// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	class UStaticMeshComponent* Body;
	class UStaticMeshComponent* Left;
	class UStaticMeshComponent* Right;
	class UBoxComponent* Box;
	class UArrowComponent* Arrow;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	*/
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;	

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("몸체"));
	Body->SetupAttachment(Box);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_P38(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_P38.Succeeded())
	{
		Body->SetStaticMesh(SM_P38.Object);
	}

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);
	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}
	Left->SetRelativeLocation(FVector(37, 21, 0));
	Right->SetRelativeLocation(FVector(37, -21, 0));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(100, 0, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Body);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	// 1. 블루 프린트 에셋으로 생성하는 경우 사용
	static ConstructorHelpers::FClassFinder<AMyRocket> BP_Rocket(TEXT("Blueprint'/Game/Blueprints/BP_MyRocket.BP_MyRocket_C'"));
	if (BP_Rocket.Succeeded())
	{
		Rocket = BP_Rocket.Class;
		UE_LOG(LogClass, Warning, TEXT("Load BP Class"));
	}
}

void AMyPawn::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
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

	RotatePropeller(Left, RotateSpeed);
	RotatePropeller(Right, RotateSpeed);

}

void AMyPawn::RotatePropeller(UMeshComponent* MeshComponent, float Speed)
{
	MeshComponent->AddLocalRotation(FRotator(0, 0, Speed*UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPawn::Fire);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
	PlayerInputComponent->BindAxis(TEXT("Boost"), this, &AMyPawn::Boost);
	
}

void AMyPawn::Pitch(float Value)
{
	if (Value != 0.0f)
	{
		AddActorLocalRotation(FRotator(RotateSpeed * Value * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),0, 0));
	}
}

void AMyPawn::Roll(float Value)
{
	if (Value != 0.0f)
	{
		AddActorLocalRotation(FRotator(0,0, RotateSpeed * Value * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
	}
}

void AMyPawn::Boost(float Value)
{
	if (Value != 0.0f)
	{
		Movement->AddInputVector(GetActorForwardVector()*MoveSpeed * Value);
	}
}

void AMyPawn::Fire()
{
//	FActorSpawnParameters F;

	// C++ class 로 바로 생성하는경우
//	GetWorld()->SpawnActor<AMyRocket>(Arrow->GetComponentLocation(), Arrow->GetComponentRotation()); 

	// 변로수 생성
	GetWorld()->SpawnActor<AMyRocket>(Rocket,Arrow->GetComponentLocation(), Arrow->GetComponentRotation()); // C++ class
}

void AMyPawn::BlueprintNative_Implementation()
{
	UE_LOG(LogClass, Warning, TEXT("C++ Implementaion"));
}