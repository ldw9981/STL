// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/BasicPlayerCameraManager.h"
#include "UI/Items/ItemToolTipBase.h"
#include "Components/TextBlock.h"
#include "Items/InventoryBase.h"
#include "Kismet/GameplayStatics.h"
#include "BDGameInstance.h"
#include "Items/InventorySystem.h"
#include "Items/ItemSlotBase.h"
#include "Battle/BattleWidgetBase.h"
#include "BDGameInstance.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = ABasicPlayerCameraManager::StaticClass();
}

void ABasicPC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		//UI ����(UMG �������Ʈ Ŭ������ �ε�, ����)
		FStringClassReference ItemToolTipClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ItemToolTip.ItemToolTip_C'"));

		//Ŭ���� �ε� �õ�
		UClass* WidgetClass = ItemToolTipClass.TryLoadClass<UItemToolTipBase>();
		if (WidgetClass) //Ŭ���� �ε� ����
		{
			//Ŭ������ �ν��Ͻ� �����
			ItemToolTip = Cast<UItemToolTipBase>(CreateWidget<UUserWidget>(this, WidgetClass));
			//ȭ�鿡 ���̱�
			if (ItemToolTip)
			{
				ItemToolTip->AddToViewport();
				ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		FStringClassReference InventoryClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Inventory.Inventory_C'"));
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

		if (BattleWidgetClass)
		{
			BattleWidget = CreateWidget<UBattleWidgetBase>(this, BattleWidgetClass);
			if (BattleWidget)
			{
				BattleWidget->AddToViewport();
			}
		}

		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			UserID = GI->UserID;
			C2S_SetUserID(UserID);
		}
	}
}

void ABasicPC::ShowItemToolTip(bool bShow)
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	if (bShow)
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
	if (IsLocalPlayerController())
	{
		ItemToolTip->ItemName->SetText(FText::FromString(ItemName));
	}
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
		UpdateInventory();
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

//Data(GI->Inventory) - UI(Widget, InventoryBase, ItemSlotBase) ����
void ABasicPC::UpdateInventory()
{
	InventoryWidget->AllResetSlot();

	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		for (int i = 0; i < GI->Inventory->ItemList.Num(); ++i)
		{
			UItemSlotBase* Slot = InventoryWidget->GetEmptySlot();
			if (Slot)
			{
				Slot->SetItemData(GI->Inventory->ItemList[i], i);
			}
			else
			{
				//�κ��丮 Ǯ
			}
		}
	}
	
}

bool ABasicPC::C2S_SetUserID_Validate(const FString & NewUerID)
{
	return true;
}

void ABasicPC::C2S_SetUserID_Implementation(const FString & NewUerID)
{
	UserID = NewUerID;
}

void ABasicPC::AllSendKillingMessage(const FString& Message)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{

	}
}