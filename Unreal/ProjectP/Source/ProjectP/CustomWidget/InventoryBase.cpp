// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryBase.h"
#include "Components/ScrollBox.h"
#include "CustomWidget/ItemSlotBase.h"

void UInventoryBase::NativeStruct()
{
	UScrollBox* Scroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("InventoryScroll")));
	if (Scroll)
	{
		for (int i = 0; i < Scroll->GetChildrenCount(); ++i)
		{
			UItemSlotBase* Slot = Cast<UItemSlotBase>(Scroll->GetChildAt(i));
			if (Slot)
			{
				Slots.Add(Slot);
				Slot->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}
