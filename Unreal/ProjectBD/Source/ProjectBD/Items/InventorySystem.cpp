// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem.h"
#include "Items/MasterItem.h"
#include "Engine/World.h"
#include "Basic/BasicPC.h"
#include "Kismet/GameplayStatics.h"
#include "Basic/BasicCharacter.h"


bool UInventorySystem::AddItem(FItemDataTable Item)
{
	//�κ��丮 Ǯ üũ

	if (Item.IsOverlapItemType())
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

bool UInventorySystem::UseItem(int Index)
{
	if (Index == -1)
	{
		return false;
	}

	if (ItemList[Index].ItemCount == 0)
	{
		return false;
	}


	if (ItemList[Index].IsOverlapItemType())
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


bool UInventorySystem::DropItem(int Index)
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

int UInventorySystem::GetSameItemIndex(FItemDataTable Item)
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
