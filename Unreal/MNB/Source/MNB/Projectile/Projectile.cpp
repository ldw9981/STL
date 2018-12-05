// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Kismet/GameplayStatics.h"
#include "CustomDamageType/CustomDamageType.h"

// Sets default values
AProjectile::AProjectile()
{	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);

	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	//DamageType = CreateDefaultSubobject<UDamageType>(TEXT("DamageType"));
	//UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{	
	Super::BeginPlay();
	Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnComponentHit);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnComponentBeginOverlap);
	//UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PrevLocation = CurrLocation;
	CurrLocation = Collision->GetComponentLocation();
}

void AProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	
	//UE_LOG(LogClass, Warning, TEXT("AProjectile::OnComponentHit: %s %s %s"), *OtherActor->GetName(), *OtherComp->GetName(), *Hit.BoneName.ToString());

	// �΋H���� �ø��� ��  
	if (DoDisableCollision)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	TArray<AActor*> IgnoreActors;
	// ������Ÿ�� ������ �������� �߻���Ų ����
	if ( Instigator != nullptr && Instigator->IsValidLowLevel() && OtherActor != Instigator)
	{
		// nullptr != ĳ�����Ͽ� UCustomDamageType �������� , nullptr�̸� UCustomDamageType�� CDO �������� 
		UCustomDamageType const* const DamageTypeCDO = CustomDamageTypeClass ? CustomDamageTypeClass->GetDefaultObject<UCustomDamageType>() : GetDefault<UCustomDamageType>();
		switch (DamageTypeCDO->CustomDamageEventType)
		{
		case ECustomDamageEventType::Point:
			{
				FVector Dir = CurrLocation - PrevLocation;
				Dir.Normalize();
				UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamage, Dir, Hit, nullptr, this, CustomDamageTypeClass);
				break;
			}
		case ECustomDamageEventType::Radial:
			UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage, Hit.Location, RadialDamageRadius, CustomDamageTypeClass, IgnoreActors, this);
			break;
		default:
			UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, nullptr, this, CustomDamageTypeClass);
			break;
		}
	}
	// üũ�� �ڽ��� ��Ʈ�� ��Ʈ ��� ���δ�.
	
	if (DoStuckOnCharacter)
	{
		AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
	}
}

void AProjectile::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//UE_LOG(LogClass, Warning, TEXT("AProjectile::OnComponentBeginOverlap: %s %s %s"),*OtherActor->GetName(),*OtherComp->GetName(), *SweepResult.BoneName.ToString());
	
	// �΋H���� �ø��� ��	
	if (DoDisableCollision)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	

	TArray<AActor*> IgnoreActors;
	// ������Ÿ�� ������ �������� �߻���Ų ����
	if (Instigator != nullptr && Instigator->IsValidLowLevel() && OtherActor != Instigator)
	{
		// nullptr != ĳ�����Ͽ� UCustomDamageType �������� , nullptr�̸� UCustomDamageType�� CDO �������� 
		UCustomDamageType const* const DamageTypeCDO = CustomDamageTypeClass ? CustomDamageTypeClass->GetDefaultObject<UCustomDamageType>() : GetDefault<UCustomDamageType>();
		switch (DamageTypeCDO->CustomDamageEventType)
		{
		case ECustomDamageEventType::Point:
			{
				FVector Dir = CurrLocation - PrevLocation;
				Dir.Normalize();
				UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamage, Dir, SweepResult, nullptr, this, CustomDamageTypeClass);
				break;
			}
		case ECustomDamageEventType::Radial:
			UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage, SweepResult.Location, RadialDamageRadius, CustomDamageTypeClass, IgnoreActors, this);
			break;
		default:
			UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, nullptr, this, CustomDamageTypeClass);
			break;
		}
	}
	// üũ�� �ڽ��� ��Ʈ�� ��Ʈ ��� ���δ�.
	if (DoStuckOnCharacter)
	{
		AttachToComponent(SweepResult.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, SweepResult.BoneName);
	}
}