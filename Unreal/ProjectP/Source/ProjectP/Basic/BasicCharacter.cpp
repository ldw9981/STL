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
#include "Materials/MaterialInstance.h"
#include "Components/DecalComponent.h"
#include "MyCameraShake.h"
#include "BulletDamageType.h"

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

	//앉을때 눈 높이 갑자기 이동 막기
	CapsuleCrouchHalfHeight = GetCharacterMovement()->CrouchedHalfHeight;
	GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	NormalSpringPosition = SpringArm->GetRelativeTransform().GetLocation();
	CrouchSpringPosition = SpringArm->GetRelativeTransform().GetLocation() - FVector(0, 0, 44);

	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();	
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	CurrentHP = MaxHP;
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
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ABasicCharacter::Reload);
}

// DamageAmount 0이면 호출되지않음
float ABasicCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))	//범위 데미지
	{
		UE_LOG(LogClass, Warning, TEXT("TakeDamage FRadialDamageEvent: %f"), DamageAmount);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))	//점 데미지
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);		
		UE_LOG(LogClass, Warning, TEXT("TakeDamage FPointDamageEvent: %f %s"), DamageAmount,*(PointDamageEvent->HitInfo.BoneName.ToString()) );


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
	}

	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		/*
		// 애니를 안쓸경우 처리방법중 하나
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//SetLifeSpan(10.0f);  //네트워크주의
		//관전자폰 세팅
		*/
		
		// 죽는 애니를 쓴다.
		//PlayAnimMontage(DeadAnimation);
		FString DeadMontage = FString::Printf(TEXT("Death_%d"), FMath::RandRange(1, 3));
		PlayAnimMontage(DeadAnimation,1.0f,FName(*DeadMontage));
	}

	return DamageAmount;
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

	//탄창이 비어있음면
	if (Weapon->IsEmptyMagazine())
	{
		//빈총소리
		return;
	}

	if (!Weapon->DecreaseMagazine())
		return;
	
	// Muzzle Flash
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash,
		Weapon->GetSocketTransform(TEXT("MuzzleFlash")));

	// FireSound
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound,
		Weapon->GetSocketLocation(TEXT("MuzzleFlash")),
		Weapon->GetSocketRotation(TEXT("MuzzleFlash")));


	//총알 발사 계산
	//UE_LOG(LogClass, Warning, TEXT("OnFire"));
	//카메라 위치랑 회전 구하기
	FVector CameraLocation;
	FRotator CameraRotation;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	//화면 사이즈 구하기
	int SizeX;
	int SizeY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(SizeX, SizeY);

	//화면상 2D 표적점을 3D 좌표 변환
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// 총알 집탄 랜덤
	int RandX = FMath::RandRange(-10, 10);
	int RandY = FMath::RandRange(-10, 10);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(
		SizeX / 2 + RandX, SizeY / 2 + RandY, CrosshairWorldPosition, CrosshairWorldDirection);

	//광선 시작점과 끝 구하기
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CrosshairWorldDirection * 900000.0f);

	//enum class EObjectType
	//vector<ObjectType> objectType;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult OutHit;

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
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
		3.0f
	);

	// 라인 트레이싱 성공이면 
	if (Result)
	{
		//광선 시작점과 끝 구하기
		TraceStart = Weapon->GetSocketLocation(TEXT("MuzzleFlash"));
		FVector Dir = OutHit.ImpactPoint - TraceStart;
		TraceEnd = TraceStart + (Dir * 1.1f);

		Result = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
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
			3.0f
		);

		UDecalComponent* ResultDecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),BulletDecal, FVector(5, 5, 5), OutHit.ImpactPoint,OutHit.ImpactNormal.Rotation(),10.0f);
		// 화면의 퍼센트 값을 사용
		ResultDecalComponent->SetFadeScreenSize(0.005f);

		if (OutHit.GetActor()->ActorHasTag(TEXT("Player")))
		{
			// HitEffect
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect,
				OutHit.ImpactPoint,
				OutHit.ImpactNormal.Rotation());
		}
		else
		{
			// HitEffect
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,
				OutHit.ImpactPoint,
				OutHit.ImpactNormal.Rotation());
		}

		// 일반 데미지
		// UGameplayStatics::ApplyDamage(OutHit.GetActor(),Weapon->GetDamage(),GetController(),this,UBulletDamageType::StaticClass());

		// 범위 데미지 (수류탄)
		/*
		//범위 공격
		//UGameplayStatics::ApplyRadialDamage(GetWorld(),
		//	Weapon->GetDamage(),
		//	OutHit.ImpactPoint,
		//	500.0f,
		//	UBulletDamageType::StaticClass(),
		//	IgnoreActors,
		//	this,
		//	GetController(),
		//	true,
		//	ECollisionChannel::ECC_Visibility
		//);
		*/

		// 점 데미지 방향성을 갖는다.
		UGameplayStatics::ApplyPointDamage(OutHit.GetActor(),
			Weapon->GetDamage(),
			TraceEnd - TraceStart,
			OutHit,
			GetController(),
			this,
			UBulletDamageType::StaticClass());
	}
	
	// 총구 방향 변경
	FRotator PlayerRotation = GetControlRotation();
	PlayerRotation.Pitch += FMath::Abs((float)RandY / 10.0f);
	PlayerRotation.Yaw += (float)RandX / 10.0f;
	GetController()->SetControlRotation(PlayerRotation);

	// 카메라 흔들기
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(UMyCameraShake::StaticClass());

	// 월드 카메라 흔들기
	// UGameplayStatics::PlayWorldCameraShake(GetWorld(), UMyCameraShake::StaticClass(), FVector(0, 0, 0), 300.0f, 900.0f);


	//연사 구현
	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle,
		this,
		&ABasicCharacter::OnFire,
		0.1f
		);

}

void ABasicCharacter::Reload()
{
	/*
	if (Weapon->ReloadComplete())
	{
		UE_LOG(LogClass, Warning, TEXT("ChangeMegazine!"));
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Not enough bullet!"));
	}
	*/
	//PlayAnimMontage(ReloadAnimation);
	bIsReload = true;
}