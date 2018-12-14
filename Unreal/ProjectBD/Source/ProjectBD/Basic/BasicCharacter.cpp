﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Basic/WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstance.h"
#include "Components/DecalComponent.h"
#include "Basic/MyCameraShake.h"
#include "Basic/BulletDamageType.h"
#include "Items/MasterItem.h"
#include "Basic/BasicPC.h"
#include "BDGameInstance.h"
#include "Items/InventorySystem.h"
#include "Battle/BattleWidgetBase.h"
#include "UnrealNetwork.h"
#include "Battle/BattleGM.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->TargetArmLength = 120.f;
	SpringArm->SetRelativeLocation(FVector(0, 32, 72));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SpringArm->bUsePawnControlRotation = true;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("RHandWeapon"));

	//앉을때 눈 높이 갑자기 이동 막기
	CapsuleCrouchHalfHeight = GetCharacterMovement()->CrouchedHalfHeight;
	GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	NormalSpringPosition = SpringArm->GetRelativeTransform().GetLocation();
	CrouchSpringPosition = SpringArm->GetRelativeTransform().GetLocation() - FVector(0, 0, 44);

	Tags.Add(TEXT("Player"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;

	SetHPBar();

	ABasicPC* PC = Cast<ABasicPC>(GetController());
	if (PC)
	{
		PC->bAlive = true;
	}
}

void ABasicCharacter::SetHPBar()
{
	ABasicPC* PC = Cast<ABasicPC>(GetController());
	if (PC && PC->IsLocalPlayerController() && PC->BattleWidget)
	{
		PC->BattleWidget->HpBarData = CurrentHP / MaxHP;
	}
}




// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasicCharacter::CheckItem()
{
	if (PickupItemList.Num() > 0)
	{
		FVector Location = GetSightLocation();

		int Index = GetClosestItem(Location);

		if (Index != -1)
		{
			ABasicPC* PC = Cast<ABasicPC>(GetController());
			if (PC)
			{
				PC->SetItemToolTipName(PickupItemList[Index]->ItemData.ItemName);
				PC->ShowItemToolTip(true);
			}
		}
	}
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

	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Pressed, this, &ABasicCharacter::DoIronsight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABasicCharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABasicCharacter::StartFire);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ABasicCharacter::StopFire);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ABasicCharacter::Reload);

	//기울여서 보기
	PlayerInputComponent->BindAction(TEXT("LeftLean"), IE_Released, this, &ABasicCharacter::StopLeftLean);

	PlayerInputComponent->BindAction(TEXT("LeftLean"), IE_Pressed, this, &ABasicCharacter::StartLeftLean);

	PlayerInputComponent->BindAction(TEXT("RightLean"), IE_Released, this, &ABasicCharacter::StopRightLean);

	PlayerInputComponent->BindAction(TEXT("RightLean"), IE_Pressed, this, &ABasicCharacter::StartRightLean);

	PlayerInputComponent->BindAction(TEXT("Use"), IE_Pressed, this, &ABasicCharacter::Use);

	PlayerInputComponent->BindAction(TEXT("ToggleInventory"), IE_Pressed, this, &ABasicCharacter::ToggleInventory);

}

void ABasicCharacter::Reload()
{
	bIsReload = true;
	//PlayAnimMontage(ReloadAnimation);
	//Weapon->ReloadComplete();
}



void ABasicCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		if (Value == -1.0f && bIsSprint)
		{
			//SetRun();
			C2S_SetSprint(false);
		}
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
//		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ABasicCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		if (bIsSprint)
		{
			//SetRun();
			C2S_SetSprint(false);
		}
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
//		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ABasicCharacter::LookUp(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerPitchInput(Value);
	}
}

void ABasicCharacter::Turn(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value);
	}
}

void ABasicCharacter::Sprint()
{
	C2S_SetSprint(true);
}

void ABasicCharacter::UnSprint()
{
	C2S_SetSprint(false);
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




FRotator ABasicCharacter::GetAimOffset() const
{
	//const FVector AimDirWS = GetBaseAimRotation().Vector();
	//const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	//const FRotator AimRotLS = AimDirLS.Rotation();

	//return AimRotLS;

	return ActorToWorld().InverseTransformVectorNoScale(GetBaseAimRotation().Vector()).Rotation();
}

void ABasicCharacter::IsFire_OnRep()
{
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
}

void ABasicCharacter::StartFire()
{
	if (!bIsReload)
	{
		bIsFire = true;
		OnTimerFire();
	}
}

void ABasicCharacter::StopFire()
{
	bIsFire = false;
}

void ABasicCharacter::OnTimerFire()
{
	if (!bIsFire)
	{
		return;
	}

	if (!Weapon->IsHaveBullet())
	{
		//빈총 소리
		bIsFire = false;
		return;
	}

	UE_LOG(LogClass, Warning, TEXT("Bullet %d %d"), Weapon->BulletCountinMagazine, Weapon->TotalBulletCount);

	

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

	//총알 범위 랜덤
	int RandX = FMath::RandRange(-10, 10);
	int RandY = FMath::RandRange(0, 10);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(SizeX / 2 + RandX, SizeY / 2 + RandY, CrosshairWorldPosition, CrosshairWorldDirection);

	//광선 시작점과 끝 구하기
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CrosshairWorldDirection * 900000.0f);


	C2S_Fire(TraceStart, TraceEnd);

	//UE_LOG(LogClass, Warning, TEXT("%d %f %s"), int, float, *FString);

	//총구 방향 변경
	FRotator PlayerRotation = GetControlRotation();
	PlayerRotation.Pitch += FMath::Abs((float)RandY / 10.0f);
	PlayerRotation.Yaw += (float)RandX / 10.0f;
	GetController()->SetControlRotation(PlayerRotation);

	//카메라 흔들기
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(UMyCameraShake::StaticClass());

	//월드 카메라 흔들기
	//UGameplayStatics::PlayWorldCameraShake(GetWorld(),
	//	UMyCameraShake::StaticClass(), FVector(0, 0, 0), 300.0f, 900.0f);

	//연사 구현
	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle,
		this,
		&ABasicCharacter::OnTimerFire,
		0.12f
	);
}

float ABasicCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentHP <= 0)
	{
		return 0;
	}

	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) //범위데미지
	{
		UE_LOG(LogClass, Warning, TEXT("Radial Damage %f"), DamageAmount);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) //점데미지
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Point Damage %f %s"), DamageAmount, *(PointDamageEvent->HitInfo.BoneName.ToString()));

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID)) //일반데미지
	{
		UE_LOG(LogClass, Warning, TEXT("Damage %f"), DamageAmount);
		CurrentHP -= DamageAmount;
	}

	// HP는 모두에게 복제가되지만, 바로 일어나지 않으므로 각 클라이언트는 HP_OnRep에서 결과처리게하고
	HP_OnRep(); // 서버는 바로 처리한다.

	if (CurrentHP <= 0)
	{

		ABattleGM* GM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			if (GM->CheckFinish())
			{

			}
		}
	}

	return DamageAmount;
}

void ABasicCharacter::StartLeftLean()
{
	C2S_SetLeftLean(true);
}

void ABasicCharacter::StopLeftLean()
{
	C2S_SetLeftLean(false);
}

void ABasicCharacter::StartRightLean()
{
	C2S_SetRightLean(true);
}

void ABasicCharacter::StopRightLean()
{
	C2S_SetRightLean(false);
}

bool ABasicCharacter::IsDead()
{
	return CurrentHP <= 0;
}

void ABasicCharacter::AddPickupItem(AMasterItem * NewItem)
{
	if (NewItem && !NewItem->IsPendingKill())
	{
		PickupItemList.Add(NewItem);
		ABasicPC* PC = Cast<ABasicPC>(GetController());

		GetWorld()->GetTimerManager().ClearTimer(ItemCheckHandle);
		GetWorld()->GetTimerManager().SetTimer(
			ItemCheckHandle,
			this,
			&ABasicCharacter::CheckItem,
			0.1f,
			true,
			0);
	}
}

void ABasicCharacter::RemovePickupItem(AMasterItem * NewItem)
{
	if (NewItem && !NewItem->IsPendingKill())
	{
		PickupItemList.Remove(NewItem);
		ABasicPC* PC = Cast<ABasicPC>(GetController());

		if (PickupItemList.Num() > 0)
		{
			FVector Location = GetSightLocation();

			int Index = GetClosestItem(Location);

			if (Index != -1)
			{
				ABasicPC* PC = Cast<ABasicPC>(GetController());
				if (PC)
				{
					PC->SetItemToolTipName(PickupItemList[Index]->ItemData.ItemName);
					PC->ShowItemToolTip(true);
				}
			}
		}
		else
		{
			PC->ShowItemToolTip(false);
			GetWorld()->GetTimerManager().ClearTimer(ItemCheckHandle);
		}
	}
}


//나랑 제일 가까운 아이템 인덱스 가져오기
int ABasicCharacter::GetClosestItem(FVector SightLocation)
{
	float Min = 99999999999.9f;
	int Index = -1;
	for (int i = 0; i < PickupItemList.Num(); ++i)
	{
		float Distance = FVector::Distance(SightLocation, PickupItemList[i]->GetActorLocation());
		if (Min > Distance)
		{
			Min = Distance;
			Index = i;
		}
	}

	return Index;
}

FVector ABasicCharacter::GetSightLocation()
{
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

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(SizeX / 2, SizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

	//광선 시작점과 끝 구하기
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CrosshairWorldDirection * 900000.0f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult OutHit;
	bool bResult =  UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		TraceStart,
		TraceEnd,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		OutHit,
		true);

	FVector Result = FVector(0, 0, 0);

	if (bResult)
	{
		Result = OutHit.ImpactPoint;
	}

	return Result;

}

void ABasicCharacter::Use()
{
	//Item 추가
	if (PickupItemList.Num() > 0)
	{
		AMasterItem* PickupItem = PickupItemList[GetClosestItem(GetSightLocation())];
		if (PickupItem && !PickupItem->IsPendingKill())
		{
			UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI)
			{
				if ( GI->Inventory->AddItem(PickupItem->ItemData) )
				{
					RemovePickupItem(PickupItem);
					PickupItem->Destroy();
					ABasicPC* PC = Cast<ABasicPC>(GetController());
					if (PC)
					{
						PC->UpdateInventory();
					}
				}
				else
				{
					//Inventory Full;
				}
			}
		}
	}
}

void ABasicCharacter::ToggleInventory()
{
	ABasicPC* PC = Cast<ABasicPC>(GetController());
	if (PC)
	{
		PC->ToggleInventory();
	}
}

void ABasicCharacter::DropItem(FItemDataTable ItemData)
{
	if (MasterItem)
	{
		AMasterItem* Item = GetWorld()->SpawnActor<AMasterItem>(MasterItem, GetMesh()->GetComponentLocation() + GetActorForwardVector() * 30.0f,
			GetMesh()->GetComponentRotation());
		//if (Item)
		//{
		//	Item->SetActorLocation(FVector(0, 0, 0));
		//}
	}
}

void ABasicCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasicCharacter, CurrentHP);
	DOREPLIFETIME(ABasicCharacter, MaxHP);
	DOREPLIFETIME(ABasicCharacter, bIsIronsight);
	DOREPLIFETIME(ABasicCharacter, bIsFire);
	DOREPLIFETIME(ABasicCharacter, bIsSprint);
	DOREPLIFETIME(ABasicCharacter, bLeftLean);
	DOREPLIFETIME(ABasicCharacter, bRightLean);
}

void ABasicCharacter::DoIronsight()
{
	C2S_DoIronsight();
}

bool ABasicCharacter::C2S_DoIronsight_Validate()
{
	return true;
}

void ABasicCharacter::C2S_DoIronsight_Implementation()
{
	bIsIronsight = bIsIronsight ? false : true;
	S2A_SetMaxWalkSpeed(bIsIronsight ? CrouchSpeed : RunSpeed);
}

bool ABasicCharacter::C2S_SetSprint_Validate(bool Sprint)
{
	return true;
}

void ABasicCharacter::C2S_SetSprint_Implementation(bool Sprint)
{
	bIsSprint = Sprint;
	S2A_SetMaxWalkSpeed(bIsSprint ? SprintSpeed : RunSpeed);
}

bool ABasicCharacter::C2S_SetLeftLean_Validate(bool NewLean)
{
	return true;
}

void ABasicCharacter::C2S_SetLeftLean_Implementation(bool NewLean)
{
	bLeftLean = NewLean;
}

bool ABasicCharacter::C2S_SetRightLean_Validate(bool NewLean)
{
	return true;
}

void ABasicCharacter::C2S_SetRightLean_Implementation(bool NewLean)
{
	bRightLean = NewLean;
}

bool ABasicCharacter::C2S_SetFire_Validate(bool NewFire)
{
	return true;
}

void ABasicCharacter::C2S_SetFire_Implementation(bool NewFire)
{
	bIsFire = NewFire;
}

void ABasicCharacter::S2A_SetMaxWalkSpeed_Implementation(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

bool ABasicCharacter::C2S_Fire_Validate(FVector TraceStart, FVector TraceEnd)
{
	return true;
}

void ABasicCharacter::C2S_Fire_Implementation(FVector TraceStart, FVector TraceEnd)
{
	if (!Weapon->UseBullet())
	{
		bIsFire = false;
		return;
	}

	S2A_FireEffect(TEXT("MuzzleFlash"));

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
		EDrawDebugTrace::None,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		3.0f
	);

	//총알이 어디에 맞으면 총구에서 다시 검사.
	//맞긴 맞은건데 이 위치가 맞는지 확인
	if (Result)
	{
		//총구 끝에서 충돌지점까지 재 검사
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
			EDrawDebugTrace::None,
			OutHit,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			3.0f
		);

		if (Result)
		{
			S2A_HitEffect(OutHit);
			//점 데미지
			UGameplayStatics::ApplyPointDamage(OutHit.GetActor(),
				Weapon->GetDamage(),
				TraceEnd - TraceStart,
				OutHit,
				GetController(),
				this,
				UBulletDamageType::StaticClass()
			);
		}
	}
}

void ABasicCharacter::S2A_HitEffect_Implementation(FHitResult OutHit)
{
	if (OutHit.GetActor()->ActorHasTag(TEXT("Player")))
	{
		//HitEffect
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect,
			OutHit.ImpactPoint,
			OutHit.ImpactNormal.Rotation()
		);
	}
	else
	{
		UDecalComponent* BulletDecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
			BulletDecal,
			FVector(5, 5, 5),
			OutHit.ImpactPoint,
			OutHit.ImpactNormal.Rotation(),
			10.0f
		);
		BulletDecalComponent->SetFadeScreenSize(0.005f);

		//HitEffect
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,
			OutHit.ImpactPoint,
			OutHit.ImpactNormal.Rotation()
		);
	}
}

void ABasicCharacter::S2A_FireEffect_Implementation(FName InSocketName)
{
	//MuzzleFlash
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash,
		Weapon->GetSocketTransform(InSocketName));

	//FireSound
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound,
		Weapon->GetSocketLocation(InSocketName),
		Weapon->GetSocketRotation(InSocketName));

}


void ABasicCharacter::HP_OnRep()
{
	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//SetLifeSpan(10.0f);
		//관전자폰 세팅
		//FString DeadMontage = FString::Printf(TEXT("Death_%d"), FMath::RandRange(1, 3));
		//PlayAnimMontage(DeadAnimation , 1.0f, FName(*DeadMontage));

		ABasicPC* PC = Cast<ABasicPC>(GetController());
		if (PC)
		{
			PC->bAlive = false;
			PC->DisableInput(PC);
		}
	}
	SetHPBar();
}