// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Basic/WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Basic/BasicPlayerCameraManager.h"
#include "Kismet/GameplayStatics.h" //Static(Global) Utiltiy
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstance.h"
#include "Components/DecalComponent.h"
#include "Zombie/ZombieAIController.h"
#include "ConstructorHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
AZombieCharacter::AZombieCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FClassFinder<AAIController> BP_ZombieAIController(TEXT("Blueprint'/Game/Blueprints/Zombie/BP_ZombieAIController.BP_ZombieAIController_C'"));
	if (BP_ZombieAIController.Succeeded())
	{
		AIControllerClass = BP_ZombieAIController.Class;
	}

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 400.0f;
	PawnSensing->SetPeripheralVisionAngle(60);
	PawnSensing->HearingThreshold = 1000.0f;
	PawnSensing->SensingInterval = 0.1f;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();	
	PawnSensing->OnSeePawn.AddDynamic(this, &AZombieCharacter::OnSeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &AZombieCharacter::OnHearNoise);
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AZombieCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	return 0.0f;
}

void AZombieCharacter::OnSeePawn(APawn * Pawn)
{
	UE_LOG(LogClass, Warning, TEXT("OnSeePawn"));
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC && AIC->BBComponent && CurrentState == EZombieState::Normal)
	{
		SetState(EZombieState::Chase);
		AIC->BBComponent->SetValueAsObject(FName(TEXT("ChaseTargetActor")),Pawn);
	}
}

void AZombieCharacter::OnHearNoise(APawn * Pawn, const FVector & Location, float Volume)
{
	UE_LOG(LogClass, Warning, TEXT("OnHearNoise"));
}

bool AZombieCharacter::SetSpeed(EZombieState NewState)
{
	switch (CurrentState)
	{
		case EZombieState::Normal:
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			return true;
		}
		case  EZombieState::Chase:
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			return true;
		}
		default:
		{
			GetCharacterMovement()->MaxWalkSpeed = 0;
			return true;
		}
	}
	return false;
}

void AZombieCharacter::SetState(EZombieState NewState)
{
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC && AIC->BBComponent)
	{
		CurrentState = EZombieState::Chase;
		AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
	}
}

