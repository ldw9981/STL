// Fill out your copyright notice in the Description page of Project Settings.

#include "BillboardWidgetComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h" //Static(Global) Utiltiy
UBillboardWidgetComponent::UBillboardWidgetComponent()
{
	
}

void UBillboardWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (GetOwner()->WasRecentlyRendered())
	{
		if (PlayerCameraManager)
		{
			SetWorldRotation(PlayerCameraManager->GetCameraRotation().Add(0, 180, 0));
		}		
	}
}
