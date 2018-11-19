// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"

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

}

void AProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogClass, Warning, TEXT(__FUNCTION__));	

	// �΋H���� �ø��� ��   , ȭ���� ���̾� ������ StaticMesh�� �ø��� ������ �����Ѵ�.
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character)
	{
		return;
	}
	//������ ó��

	
	// üũ�� �ڽ��� ��Ʈ�� ��Ʈ ��� ���δ�.
	if (!DoStuckOnCharacter)
	{
		return;
	}
	AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
	UE_LOG(LogClass, Warning, TEXT("%s"), *(Hit.Actor->GetName()));
}