// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

/*
   ACharacter ��� �Ļ�Ŭ������ ����� Weapon StaicMesh�� ����� ��������ʰ� Actor��� �Ļ�Ŭ������ ����� ACharacter�� ����ġ�Ѵ�.  ������ 
   1.  ���� �ΰ�����( ������, �Ѽ�,���,���) ���´�.
   2. Actor�� SceneComponent[���� or ��� ��ǥ]�� ���� �Ͽ� �ùٸ��� Character�� Bone�� Attach�ǵ��� �Ѵ�. 
   3. Character�� ��Ʈ�� �ø������� Ȱ���Ѵ�.
   4. ����Ÿ�����μ� �ǹ̸� ���´�.
*/
UCLASS()
class MNB_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultScene;

	TWeakObjectPtr<AActor> DamageCauser = nullptr;
	bool Equiped = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AddCollision();
	void RemoveCollision();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetDamageCauser(AActor* NewDamageCauser);
	AActor* GetDamageCauser();

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
