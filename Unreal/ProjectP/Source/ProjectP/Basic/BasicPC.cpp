// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/BasicPlayerCameraManager.h"
#include "CustomWidget/ItemToolTip.h"
#include "Components/TextBlock.h"
#include "CustomWidget/InventoryBase.h"
#include "CustomWidget/ItemSlotBase.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = ABasicPlayerCameraManager::StaticClass();
}

void ABasicPC::BeginPlay()
{
	Super::BeginPlay();

	// UI���� UMG�������Ʈ Ŭ���� �ν��Ͻ� ����
	FStringClassReference ItemToolTipClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/UMG_ItemToolTip.UMG_ItemToolTip_C'"));
	// Ŭ���� ���� �ε�
	UClass* WidgetClass = ItemToolTipClass.TryLoadClass<UItemToolTip>();
	if (WidgetClass)
	{		
		ItemToolTip = Cast<UItemToolTip>(CreateWidget<UUserWidget>(this, WidgetClass));//�ν��Ͻ� ����
		if (ItemToolTip)
		{
			ItemToolTip->AddToViewport();//ȭ�鿡 ���̱�	
			ShowItemToolTip(false);	// �ϴܲ���.
		}
	}

	// UI���� UMG�������Ʈ Ŭ���� �ν��Ͻ� ����
	FStringClassReference InventoryClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/UMG_Inventory.UMG_Inventory_C'"));
	WidgetClass = InventoryClass.TryLoadClass<UInventoryBase>();
	if (WidgetClass)
	{
		InventoryWidget = Cast<UInventoryBase>(CreateWidget<UUserWidget>(this, WidgetClass));
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ABasicPC::ShowItemToolTip(bool Show)
{
	if (Show)
	{
		ItemToolTip->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void ABasicPC::SetItemToolTipName(FString ItemName)
{
	ItemToolTip->ItemName->SetText(FText::FromString(ItemName));
}

void ABasicPC::ToggleInventory()
{
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}
