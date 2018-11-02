// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicCharacter.h"
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

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0,-GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->SetRelativeLocation(FVector(0, 32, 72));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);


	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(),TEXT("RHandWeapon"));

	NormalSpringPosition = SpringArm->GetRelativeTransform().GetLocation();
	CrouchSpringPosition = SpringArm->GetRelativeTransform().GetLocation() - FVector(0, 0, 44);

}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();	
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UE_LOG(LogClass, Warning, TEXT("JumpZVelocity:%f AimYaw:%f"), GetCharacterMovement()->Velocity.Z, 0.0f);
	
}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABasicCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABasicCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABasicCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABasicCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ABasicCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ABasicCharacter::UnSprint);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ABasicCharacter::DoCrouch);
	PlayerInputComponent->BindAction(TEXT("IronSights"), IE_Pressed, this, &ABasicCharacter::DoIronSight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABasicCharacter::DoJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABasicCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ABasicCharacter::StopFire);
}
void ABasicCharacter::MoveForward(float Value)
{
	if (Value == 0.0f)
		return;

	if (Value == -1.0f && bIsSprint)
	{
		SetRun();
	}

	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)), Value);

	// AddMovementInput(GetActorForward(),Value);
}
void ABasicCharacter::MoveRight(float Value)
{
	if (Value == 0.0f)
		return;

	if (bIsSprint)
	{
		SetRun();
	}
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)), Value);
	// AddMovementInput(GetActorRight(),Value);
}
void ABasicCharacter::LookUp(float Value)
{
	if (Value == 0.0f)
		return;

	AddControllerPitchInput(Value);
}
void ABasicCharacter::Turn(float Value)
{
	if (Value == 0.0f)
		return;

	AddControllerYawInput(Value);
}

void ABasicCharacter::Sprint()
{
	SetSprint();
}

void ABasicCharacter::UnSprint()
{
	SetRun();
}
void ABasicCharacter::DoCrouch()
{
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ABasicCharacter::SetSprint()
{
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABasicCharacter::SetRun()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

FRotator ABasicCharacter::GetAimOffset()
{
	/*
	FVector AimDirWS = GetBaseAimRotation().Vector();
	FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	FRotator AimRotLS = AimDirLS.Rotation();
	return AimRotLS;
	*/
	return ActorToWorld().InverseTransformVectorNoScale(GetBaseAimRotation().Vector()).Rotation();
}

void ABasicCharacter::DoIronSight()
{
	bIronSight = !bIronSight;	

}

void ABasicCharacter::DoJump()
{
	Jump();
}

void ABasicCharacter::StartFire()
{
	bIsFire = true;
	OnFire();
}

void ABasicCharacter::StopFire()
{
	bIsFire = false;
}

void ABasicCharacter::OnFire()
{
	if (!bIsFire)
		return;

	// 총알 발사 계산
	FVector CameraLocation;
	FRotator CameraRotation;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	int SizeX, SizeY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(SizeX, SizeY);
	
	FVector CrosshairWorldLocation, CrosshairWorldDirection;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(SizeX/2,SizeY/2, CrosshairWorldLocation, CrosshairWorldDirection);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CrosshairWorldDirection*900000.0f);

	// 컬리전 속성 오브젝트 타입 확인해야함.
	TArray<TEnumAsByte<ETraceTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult OutHit;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		TraceStart, 
		TraceEnd, 
		ObjectTypes,
		true,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		3.0f);

	// 연사구현
	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle,this,&ABasicCharacter::OnFire, 0.1f);
}
