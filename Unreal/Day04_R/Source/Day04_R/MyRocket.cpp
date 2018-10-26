// Fill out your copyright notice in the Description page of Project Settings.

#include "MyRocket.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

// Sets default values
AMyRocket::AMyRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("몸체"));
	Body->SetupAttachment(Box);
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	// 메쉬연결
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(TEXT("StaticMesh'/Game/P38/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Rocket.Succeeded())
	{
		Body->SetStaticMesh(SM_Rocket.Object);
	}	
	
	Movement->InitialSpeed = 3000.0f;
	Movement->ProjectileGravityScale = 0.0f;

	Body->AddLocalRotation(FRotator(-90, 0, 0));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(TEXT("ParticleSystem'/Game/MobileStarterContent/Particles/P_Explosion.P_Explosion'"));
	if (P_Explosion.Succeeded())
	{
		ExplosionEffect = P_Explosion.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> S_Explosion(TEXT("SoundCue'/Game/MobileStarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	if (S_Explosion.Succeeded())
	{
		ExplosionSound = S_Explosion.Object;
	}
}

// Called when the game starts or when spawned
void AMyRocket::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(30.0f);

	//Box->OnComponentBeginOverlap.AddDynamic(this, &AMyRocket::OnComponentBeginOverlap);
}

// Called every frame
void AMyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyRocket::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogClass, Warning, TEXT("ComponentOverlap"));
	UE_LOG(LogClass, Warning, TEXT("Owner %s"),*GetOwner()->GetName());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		ExplosionEffect,
		GetActorTransform()
	);

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
		ExplosionSound,
		GetActorLocation()
	);

	Destroy();
}

void AMyRocket::OnComponentBeginOverlapCallable(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogClass, Warning, TEXT("OnComponentBeginOverlapCallable"));
	UE_LOG(LogClass, Warning, TEXT("Owner %s"), *GetOwner()->GetName());


	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		ExplosionEffect,
		GetActorTransform()
	);

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
		ExplosionSound,
		GetActorLocation()
	);

	Destroy();
}

