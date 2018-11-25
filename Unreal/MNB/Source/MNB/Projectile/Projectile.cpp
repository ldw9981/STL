// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Kismet/GameplayStatics.h"

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
	Movement->InitialSpeed = 3000.0f;

	//DamageType = CreateDefaultSubobject<UDamageType>(TEXT("DamageType"));
	//UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{	
	Super::BeginPlay();
	Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnComponentHit);
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
	Ticked = 2;
	//UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));	

	// �΋H���� �ø��� ��   , ȭ���� ���̾� ������ StaticMesh�� �ø��� ������ �����Ѵ�.
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character)
	{
		return;
	}
	FVector Dir = CurrLocation - PrevLocation;
	Dir.Normalize();

	TArray<AActor*> IgnoreActors;
	// �߻�ü ������ �������� ������ ó��
	if (DamageCauser.Get() != nullptr)
	{
		// nullptr != ĳ�����Ͽ� UCustomDamageType �������� , nullptr�̸� UCustomDamageType�� CDO �������� 
		UCustomDamageType const* const DamageTypeCDO = CustomDamageTypeClass ? CustomDamageTypeClass->GetDefaultObject<UCustomDamageType>() : GetDefault<UCustomDamageType>();
		switch (DamageTypeCDO->CustomDamageEventType)
		{
		case ECustomDamageEventType::Point:
			UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamage, Dir, Hit, nullptr, this, CustomDamageTypeClass);
			break;
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
	Ticked = 1;
}

void AProjectile::SetDamageCauser(AActor * NewDamageCauser)
{
	DamageCauser = NewDamageCauser;
}

AActor * AProjectile::GetDamageCauser()
{
	return DamageCauser.Get();
}
