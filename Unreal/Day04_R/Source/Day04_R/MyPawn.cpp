// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "MyRocket.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*
	class UStaticMeshComponent*
	class UStaticMeshComponent*
	class UStaticMeshComponent*
	class UArrowComponent*
	class UBoxComponent*
	class USpringArmComponent*
	class UCameraComponent*
	class UFloatingPawnMovement*
	*/

	// 컴포넌트 생성및 트리연결
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));	
	RootComponent = Box;
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("몸체"));
	Body->SetupAttachment(Box);

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);
		
	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Body);
	


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Body);


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	// 메쉬연결
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

	//기타 설정
	Box->SetBoxExtent(FVector(29, 8, 8));
	Arrow->SetRelativeLocation(FVector(100.0f, 0, 0));
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	Right->SetRelativeLocation(FVector(37, 21, 0));
	Left->SetRelativeLocation(FVector(37, -21, 0));



	// 테스트코드
	TArray<AActor*> Output;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Output); // 태크 활용하기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Output); // CDO 활용하기
	for (auto Actor : Output)
	{
		UE_LOG(LogClass, Warning, TEXT("Player %s"), *Actor->GetName());
	}
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
	float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	RotatePropeller(Left, deltaTime * 1080);
	RotatePropeller(Right, deltaTime * 1080);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Boost"), this, &AMyPawn::Boost);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed, this, &AMyPawn::Fire);

}

void AMyPawn::RotatePropeller(UStaticMeshComponent * Propeller, float Value)
{
	Propeller->AddLocalRotation(FRotator(0, 0, Value));
}

void AMyPawn::Roll(float Value)
{
	if (Value != 0.0f)
	{
		float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		AddActorLocalRotation(FRotator(0, 0, Value*90.0f*deltaTime));
	}
}

void AMyPawn::Pitch(float Value)
{
	if (Value != 0.0f)
	{
		float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		AddActorLocalRotation(FRotator(Value*90.0f*deltaTime,0, 0 ));
	}
}

void AMyPawn::Boost(float Value)
{
	if (Value != 0.0f)
	{
		float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		AddMovementInput(GetActorForwardVector(), Value*100.0f*deltaTime);
	}
}

void AMyPawn::Fire()
{
	//#include "Engine/GameEngine.h"
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));

	SpawnFireParticle();
	SpawnRocket();			// 블루프린트에서 재정의 가능한 함수를 정의하고 호출하는방법
}

void AMyPawn::SpawnRocket_Implementation()
{
	UE_LOG(LogClass, Warning, TEXT("C++ SpawnRocket_Implementation"));
	// 생성에대한 추가정보 세팅
	FActorSpawnParameters F;
	F.Owner = this;
	F.Instigator = this;

	if (Rocket == nullptr)
	{
		//UE_LOG(LogClass, Warning, TEXT("Rocket == nullptr"));		
		GetWorld()->SpawnActor<AMyRocket>(Arrow->GetComponentLocation(), Arrow->GetComponentRotation(), F);
	}
	else
	{
		//UE_LOG(LogClass, Warning, TEXT("Rocket != nullptr"));
		GetWorld()->SpawnActor<AMyRocket>(Rocket, Arrow->GetComponentLocation(), Arrow->GetComponentRotation(), F);
	}
}
