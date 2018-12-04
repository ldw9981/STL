// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem.h"


// return isfull?
bool UInventorySystem::AddItem(FItemDataTable NewItem)
{
	if (NewItem.ItemType == EItemType::Consume)
	{
		int Index = GetSameItemIndex(NewItem);
		if (Index == -1)
		{				
			ItemList.Add(NewItem);	//신규추가
		}
		else
		{
			ItemList[Index].ItemCount += NewItem.ItemCount; // 개숮ㅇ가
		}
	}
	else 
	{	// 신규추가
		ItemList.Add(NewItem);
	}

	UE_LOG(LogClass, Warning, TEXT("-----------------"));
	for (auto Item : ItemList)
	{
		UE_LOG(LogClass, Warning, TEXT("Item %s %d"), *Item.ItemName, Item.ItemCount);
	}

	return false;
}

bool UInventorySystem::UseItem(int Index)
{
	if (ItemList[Index].ItemType == EItemType::Consume)
	{
		//-- 후 0이면 목록에서 제거
		ItemList[Index].ItemCount--;
		if (ItemList[Index].ItemCount == 0)
		{
			ItemList.RemoveAt(Index);
		}
	}
	else
	{
		// 장비 착용 구현 필요
	}
	return true;
}

bool UInventorySystem::DropItem(int Index)
{
	// 현재위치에 아이템 생성 필요

	//버릴아이템 삭제
	ItemList.RemoveAt(Index);
	return true;
}

int UInventorySystem::GetSameItemIndex(FItemDataTable NewItem)
{
	int Index = -1;
	for (int i=0;i<ItemList.Num();++i)
	{
		if (ItemList[i].ItemIndex == NewItem.ItemIndex)
		{
			Index = i;
			break;
		}
	}
	return Index;
}
