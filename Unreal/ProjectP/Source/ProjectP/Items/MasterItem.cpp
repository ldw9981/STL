// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CustomComponents/ItemComponent.h"
#include "Engine/StreamableManager.h"
#include "Components/PrimitiveComponent.h"
#include "Basic/BasicCharacter.h"

// Sets default values
AMasterItem::AMasterItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(150.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);

	ItemCompo = CreateDefaultSubobject<UItemComponent>(TEXT("Item"));

}

// Called when the game starts or when spawned
void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	if (ItemCompo->ItemDataTable)
	{
		ItemIndex = FMath::RandRange(1, 6) * 10;
		ItemData = ItemCompo->GetItemData(ItemIndex);

		if (ItemData.ItemIndex != 0)
		{
			// �޽÷ε� 3���� ���
			FStreamableManager Loader;
			//1. ����
			//Mesh->SetStaticMesh(Loader.LoadSynchronous<UStaticMesh>(ItemData.ItemMesh));

			//2. �񵿱� & ����
			/*
			Loader.RequestAsyncLoad(ItemData.ItemMesh.ToSoftObjectPath(), [this]()
			{
				Mesh->SetStaticMesh(ItemData.ItemMesh.Get());
			});
			*/

			//3. �񵿱� & ����Լ�
			Loader.RequestAsyncLoad(ItemData.ItemMesh.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AMasterItem::CompleteAsyncLoad));
		}
	}

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::OnEndOverlap);
}

void AMasterItem::CompleteAsyncLoad()
{
	Mesh->SetStaticMesh(ItemData.ItemMesh.Get());
}

void AMasterItem::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		ABasicCharacter* Pawn = Cast<ABasicCharacter>(OtherActor);
		Pawn->AddPickupItem(this);
	}

}

void AMasterItem::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		ABasicCharacter* Pawn = Cast<ABasicCharacter>(OtherActor);
		Pawn->DelPickupItem(this);
	}
}

// Called every frame
void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

