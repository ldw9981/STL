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

	// UI생성 UMG블루프린트 클래스 인스턴스 생성
	FStringClassReference ItemToolTipClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/UMG_ItemToolTip.UMG_ItemToolTip_C'"));
	// 클래스 정보 로딩
	UClass* WidgetClass = ItemToolTipClass.TryLoadClass<UItemToolTip>();
	if (WidgetClass)
	{		
		ItemToolTip = Cast<UItemToolTip>(CreateWidget<UUserWidget>(this, WidgetClass));//인스턴스 생성
		if (ItemToolTip)
		{
			ItemToolTip->AddToViewport();//화면에 붙이기	
			ShowItemToolTip(false);	// 일단끈다.
		}
	}

	// UI생성 UMG블루프린트 클래스 인스턴스 생성
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
