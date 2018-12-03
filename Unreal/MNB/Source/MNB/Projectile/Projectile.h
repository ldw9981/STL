// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomDamageType/CustomDamageType.h"
#include "Projectile.generated.h"

UCLASS()
class MNB_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UProjectileMovementComponent* Movement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OnCollide")
	bool DoStuckOnCharacter = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OnCollide")
	bool DoDisableCollision = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OnCollide")
	float BaseDamage = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OnCollide")
	float RadialDamageRadius = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OnCollide")
	TSubclassOf<UCustomDamageType> CustomDamageTypeClass;


	/*
	// 리소스는 보유 여부에 따라 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect")
	class UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect")
	class USoundBase* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect")
	class UMaterialInstance* HitDecal;
	*/
	FVector	CurrLocation;
	FVector	PrevLocation;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
