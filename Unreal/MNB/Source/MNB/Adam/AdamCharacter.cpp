// Fill out your copyright notice in the Description page of Project Settings.

#include "AdamCharacter.h"
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
#include "Components/ArrowComponent.h"
#include "Projectile/Projectile.h"
#include "CustomComponents/CharacterStateComponent.h"
#include "GameFramework/Actor.h"
	//#include "Basic/WeaponComponent.h"
//#include "Basic/BasicPlayerCameraManager.h"
//#include "MyCameraShake.h"
//#include "BulletDamageType.h"


// Sets default values
AAdamCharacter::AAdamCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -88, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->SetRelativeLocation(FVector(0, 32, 72));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CharacterState = CreateDefaultSubobject<UCharacterStateComponent>(TEXT("CharacterState"));

	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void AAdamCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

// Called every frame
void AAdamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAdamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AAdamCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AAdamCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAdamCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAdamCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AAdamCharacter::DoJump);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AAdamCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AAdamCharacter::StopFire);
}

void AAdamCharacter::MoveForward(float Value)
{
	if (Value == 0.0f)
		return;

	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)), Value);

	// AddMovementInput(GetActorForward(),Value);
}
void AAdamCharacter::MoveRight(float Value)
{
	if (Value == 0.0f)
		return;

	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)), Value);
	// AddMovementInput(GetActorRight(),Value);
}
void AAdamCharacter::LookUp(float Value)
{
	if (Value == 0.0f)
		return;

	AddControllerPitchInput(Value);
}
void AAdamCharacter::Turn(float Value)
{
	if (Value == 0.0f)
		return;

	AddControllerYawInput(Value);
}

void AAdamCharacter::DoJump()
{
	Jump();
}

void AAdamCharacter::StartFire()
{
	// 블루프린트에서 설정한 화살 액터로 생성한다.
	if (Projectile != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Cast<APawn>(this);

		FVector Location = SpringArm->GetComponentLocation() + SpringArm->GetForwardVector() * 200;
		FRotator Rotation = SpringArm->GetComponentRotation();
		AProjectile* Actor = GetWorld()->SpawnActor<AProjectile>(Projectile, Location, Rotation, SpawnParams); // C++ class		
	}	
}

void AAdamCharacter::StopFire()
{

}

/*
float AAdamCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float Result =  CharacterState->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);



	return Result;
}
*/