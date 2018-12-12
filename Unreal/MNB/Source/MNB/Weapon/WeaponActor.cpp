// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ActorComponent.h"
#include "Components/ShapeComponent.h"
#include "CustomDamageType/AnyDamageType.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SetRootComponent(DefaultScene);

	
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	APawn* ParentPawn = Cast<APawn>(this->GetParentActor());
	if (ParentPawn)
	{
		Instigator = ParentPawn;
	}


	TArray <UShapeComponent *> SourceComps;
	GetComponents(SourceComps);
	for (UShapeComponent* ActorComp : SourceComps)
	{
		UShapeComponent* SourceComp = Cast<UShapeComponent>(ActorComp);
		if (SourceComp)
		{
			SourceComp->OnComponentHit.AddDynamic(this, &AWeaponActor::OnComponentHit);
			SourceComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnComponentBeginOverlap);
		}
	}
	
	SetActorEnableCollision(false);
}


// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeaponActor::OnComponentHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	UE_LOG(LogClass, Warning, TEXT("AWeaponActor::OnComponentHit: %s %s %s"), *OtherActor->GetName(), *OtherComp->GetName(), *Hit.BoneName.ToString());
	
	TArray<AActor*> IgnoreActors;
	// 프로젝타일 액터의 데미지를 발생시킨 액터
	if (Instigator != nullptr && Instigator->IsValidLowLevel() && OtherActor != Instigator)
	{
		// nullptr != 캐스팅하여 UCustomDamageType 가져오기 , nullptr이면 UCustomDamageType의 CDO 가져오기 
		UAnyDamageType const* const DamageTypeCDO = DamageTypeClass ? DamageTypeClass->GetDefaultObject<UAnyDamageType>() : GetDefault<UAnyDamageType>();
		switch (DamageTypeCDO->DamageEventType)
		{
		case EDamageEventType::Point:
			{
				FVector Dir = CurrLocation - PrevLocation;
				Dir.Normalize();
				UGameplayStatics::ApplyPointDamage(OtherActor, Damage, Dir, Hit, nullptr, this, DamageTypeClass);
				break;
			}
		case EDamageEventType::Radial:
			UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, Hit.Location, RadialRadius, DamageTypeClass, IgnoreActors, this);
			break;
		default:
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);
			break;
		}
	}	
}

void AWeaponActor::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogClass, Warning, TEXT("AWeaponActor::OnComponentBeginOverlap: %s %s %s"), *OtherActor->GetName(), *OtherComp->GetName(), *SweepResult.BoneName.ToString());
	   	  
	TArray<AActor*> IgnoreActors;
	// 데미지를 발생시킨 액터
	if (Instigator != nullptr && Instigator->IsValidLowLevel() && OtherActor != Instigator)
	{
		// nullptr != 캐스팅하여 UCustomDamageType 가져오기 , nullptr이면 UCustomDamageType의 CDO 가져오기 
		UAnyDamageType const* const DamageTypeCDO = DamageTypeClass ? DamageTypeClass->GetDefaultObject<UAnyDamageType>() : GetDefault<UAnyDamageType>();
		switch (DamageTypeCDO->DamageEventType)
		{
		case EDamageEventType::Point:
			{
				FVector Dir = CurrLocation - PrevLocation;
				Dir.Normalize();
				UGameplayStatics::ApplyPointDamage(OtherActor, Damage, Dir, SweepResult, nullptr, this, DamageTypeClass);
				break;
			}

		case EDamageEventType::Radial:
			UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, SweepResult.Location, RadialRadius, DamageTypeClass, IgnoreActors, this);
			break;
		default:
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);
			break;
		}
	}
}
