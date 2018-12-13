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


	if (GetWorld())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			ABasicCharacter* Pawn = Cast<ABasicCharacter>(PC->GetPawn());
			if (Pawn)
			{
				Pawn->DropItem(ItemList[Index]);
				//���� ������ ����
				ItemList.RemoveAt(Index);

				return true;
			}
		}
	}

	return false;
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
