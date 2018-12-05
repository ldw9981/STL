// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

/*
   ACharacter 기반 파생클래스를 만들어 Weapon StaicMesh를 멤버로 사용하지않고 Actor기반 파생클래스를 만들고 ACharacter에 어태치한다.  이유는 
   1.  무기 부가정보( 데미지, 한손,양손,등등) 갖는다.
   2. Actor가 SceneComponent[로컬 or 상대 좌표]를 갖게 하여 올바르게 Character에 Bone에 Attach되도록 한다. 
   3. Character는 루트만 컬리전으로 활용한다.
   4. 무기타입으로서 의미를 갖는다.
*/

class UCustomDamageType;

UCLASS()
class MNB_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OnCollide")
	float BaseDamage = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OnCollide")
	float RadialDamageRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OnCollide")
	TSubclassOf<UCustomDamageType> CustomDamageTypeClass;

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
