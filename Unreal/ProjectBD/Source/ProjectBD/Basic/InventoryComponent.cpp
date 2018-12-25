// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "BDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem(int ItemIndex, int Count)
{
	//�κ��丮 Ǯ üũ
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemDataTable& ItemData = GI->GetItemData(ItemIndex);
	FInventoryItemInfo InventoryItemInfo;
	InventoryItemInfo.ItemIndex = ItemIndex;
	InventoryItemInfo.ItemCount = Count;

	if (ItemData.IsOverlapItemType())
	{
		int Index = GetSameItemIndex(ItemData);
		if (Index == -1)
		{
			//�ű� �߰�
			
			ItemList.Add(InventoryItemInfo);
		}
		else
		{
			//���� ����
			ItemList[Index].ItemCount += Count;
		}
	}
	else
	{
		//�ű� �߰�
		ItemList.Add(InventoryItemInfo);
	}
	return true;
}

bool UInventoryComponent::UseItem(int InventoryIndex)
{
	if (InventoryIndex == -1)
	{
		return false;
	}

	if (ItemList[InventoryIndex].ItemCount == 0)
	{
		return false;
	}


	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FItemDataTable& ItemData = GI->GetItemData(ItemList[InventoryIndex].ItemIndex);
	if (ItemData.IsOverlapItemType())
	{
		//����ϴ°�
		ItemList[InventoryIndex].ItemCount--;
		//ȿ�� ����
		if (ItemList[InventoryIndex].ItemCount == 0)
		{
			//��� ���� �������� ����
			ItemList.RemoveAt(InventoryIndex);
		}
	}
	else
	{
		//���
	}

	return true;
}


bool UInventoryComponent::DropItem(int Index)
{
	if (Index == -1)
	{
		return false;
	}

	if (ItemList[Index].ItemCount == 0)
	{
		return false;
	}

	ItemList.RemoveAt(Index);
	return true;
}

int UInventoryComponent::GetSameItemIndex(FItemDataTable Item)
{
	for (int i = 0; i < ItemList.Num(); ++i)
	{
		if (ItemList[i].ItemIndex == Item.ItemIndex)
		{
			return i;
		}
	}

	return -1;
}

int UInventoryComponent::GetItemIndex(int InventoryIndex)
{
	if (InventoryIndex >= ItemList.Num())
	{
		return -1;
	}
	return ItemList[InventoryIndex].ItemIndex;
}


int UInventoryComponent::GetItemCount(int InventoryIndex)
{
	if (InventoryIndex >= ItemList.Num())
	{
		return 0;
	}
	return ItemList[InventoryIndex].ItemCount;
}