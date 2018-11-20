// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
//#include "Basic/BasicPlayerCameraManager.h"
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
#include "Adam/AdamCharacter.h"
#include "CharacterState/CharacterStateComponent.h"
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
	CurrentHP = MaxHP;
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
	if (CurrentHP <= 0)
		return 0;

	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))	//범위 데미지
	{
		UE_LOG(LogClass, Warning, TEXT("TakeDamage FRadialDamageEvent: %f"), DamageAmount);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))	//점 데미지
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
		UE_LOG(LogClass, Warning, TEXT("TakeDamage FPointDamageEvent: %f %s"), DamageAmount, *(PointDamageEvent->HitInfo.BoneName.ToString()));


		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;

		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))			// 일반데미지
	{
		UE_LOG(LogClass, Warning, TEXT("TakeDamage FDamageEvent: %f"), DamageAmount);
		CurrentHP -= DamageAmount;
	}

	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		SetState(EZombieState::Dead);		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetLifeSpan(10.0f);
	}

	return DamageAmount;
}

void AZombieCharacter::OnSeePawn(APawn * Pawn)
{	
	// 폰센싱컴포넌트의 Tag목록과 탐지된 폰이 가진 tag목록 교집합이 있을때만 탐지한다.
	bool IsEnemy=false;
	for (auto Tag : EnemyTags)
	{
		if (Pawn->ActorHasTag(Tag))
		{
			IsEnemy = true;
			break;
		}
	}
	if (!IsEnemy)
	{
		return;
	}

	// 상태 컴포넌트가 있고 살아있는 폰만 대상으로 한다.
	TArray <UCharacterStateComponent *> Container;
	Pawn->GetComponents(Container);
	if (Container.Num() <= 0)
	{
		return;
	}
	for (auto CharacterStateComponent : Container)
	{
		if (CharacterStateComponent->IsDead())
		{
			return;
		}
	}

	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC && AIC->BBComponent  && CurrentState == EZombieState::Normal )
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
		CurrentState = NewState;
		AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
	}
}

