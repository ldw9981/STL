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
	//인벤토리 풀 체크

	if (Item.ItemType == EItemType::Consume)
	{
		int Index = GetSameItemIndex(Item);
		if (Index == -1)
		{
			//신규 추가
			ItemList.Add(Item);
		}
		else
		{
			//갯수 증가
			ItemList[Index].ItemCount += Item.ItemCount;
		}
	}
	else
	{
		//신규 추가
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
		//사용하는거
		ItemList[Index].ItemCount--;
		//효과 적용
		if (ItemList[Index].ItemCount == 0)
		{
			//사용 다한 아이템을 삭제
			ItemList.RemoveAt(Index);
		}
	}
	else
	{
		//장비
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
