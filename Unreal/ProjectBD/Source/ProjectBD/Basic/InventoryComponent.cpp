// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

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

bool UInventoryComponent::AddItem(FItemDataTable Item)
{
	//�κ��丮 Ǯ üũ

	if (Item.ItemType == EItemType::Consume)
	{
		int Index = GetSameItemIndex(Item);
		if (Index == -1)
		{
			//�ű� �߰�
			ItemList.Add(Item);
		}
		else
		{
			//���� ����
			ItemList[Index].ItemCount += Item.ItemCount;
		}
	}
	else
	{
		//�ű� �߰�
		ItemList.Add(Item);
	}

	return true;
}

bool UInventoryComponent::UseItem(int Index)
{
	if (Index == -1)
	{
		return false;
	}

	if (ItemList[Index].ItemCount == 0)
	{
		return false;
	}


	if (ItemList[Index].ItemType == EItemType::Consume)
	{
		//����ϴ°�
		ItemList[Index].ItemCount--;
		//ȿ�� ����
		if (ItemList[Index].ItemCount == 0)
		{
			//��� ���� �������� ����
			ItemList.RemoveAt(Index);
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
