// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyRocket.generated.h"

UCLASS()
class DAY04_R_API AMyRocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyRocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UProjectileMovementComponent* Movement;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable , meta = (DisplayNAme = "충돌처리"))	//  need save as UTF8 
	void OnComponentBeginOverlapCallable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* ExplosionSound;

private:
	// 클래스에서 프라이빗 정보 이지만 블루프린트에서 보여줘야하는 경우 노출
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Test" , meta = ( AllowPrivateAccess = "true" ))
	float PrivateTest = 100;
};
