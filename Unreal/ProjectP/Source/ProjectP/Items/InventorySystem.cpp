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
			ItemList.Add(NewItem);	//�ű��߰�
		}
		else
		{
			ItemList[Index].ItemCount += NewItem.ItemCount; // ���G����
		}
	}
	else 
	{	// �ű��߰�
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
		//-- �� 0�̸� ��Ͽ��� ����
		ItemList[Index].ItemCount--;
		if (ItemList[Index].ItemCount == 0)
		{
			ItemList.RemoveAt(Index);
		}
	}
	else
	{
		// ��� ���� ���� �ʿ�
	}
	return true;
}

bool UInventorySystem::DropItem(int Index)
{
	// ������ġ�� ������ ���� �ʿ�

	//���������� ����
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
