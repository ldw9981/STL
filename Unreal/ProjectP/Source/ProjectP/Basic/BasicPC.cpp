// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/BasicPlayerCameraManager.h"
#include "CustomWidget/ItemToolTip.h"
#include "Components/TextBlock.h"

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
		//�ν��Ͻ� ����
		ItemToolTip = Cast<UItemToolTip>(CreateWidget<UUserWidget>(this, WidgetClass));

		//ȭ�鿡 ���̱�
		ItemToolTip->AddToViewport();

		// 
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