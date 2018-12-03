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

	// UI생성 UMG블루프린트 클래스 인스턴스 생성
	FStringClassReference ItemToolTipClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/UMG_ItemToolTip.UMG_ItemToolTip_C'"));

	// 클래스 정보 로딩
	UClass* WidgetClass = ItemToolTipClass.TryLoadClass<UItemToolTip>();
	if (WidgetClass)
	{
		//인스턴스 생성
		ItemToolTip = Cast<UItemToolTip>(CreateWidget<UUserWidget>(this, WidgetClass));

		//화면에 붙이기
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