// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemToolTip.h"
#include "Components/TextBlock.h"

void UItemToolTip::NativeConstruct()
{
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));

}
