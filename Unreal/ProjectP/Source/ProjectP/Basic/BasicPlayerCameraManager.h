// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasicPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API ABasicPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

		ABasicPlayerCameraManager();
public:
	virtual void BeginPlay() override;
	virtual void UpdateCamera(float DeltaTime) override;
	float NormalFOV = 90.0f;
	float IronSightFOV = 60.0f;
	FVector CurrentSpringArm;
};
