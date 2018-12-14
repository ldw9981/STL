// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGM.h"
#include "Engine/World.h"
#include "Basic/BasicPC.h"
#include "Battle/BattleGS.h"

void ABattleGM::BeginPlay()
{
	Super::BeginPlay();

	FGameModeEvents::GameModePostLoginEvent.AddUObject(this, &ABattleGM::LoginEvent);
}

void ABattleGM::LoginEvent(AGameModeBase* GameMode, APlayerController* NewPlayer)
{
	ReCountAliveCount();
}

//Player Á×À¸¸é È£Ãâ 
int ABattleGM::ReCountAliveCount()
{
	int Count = 0;
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ABasicPC* PC = Cast<ABasicPC>(*Iter);
		if (PC)
		{
			if (PC->bAlive)
			{
				Count++;
			}
		}
	}

	ABattleGS* GS = GetGameState<ABattleGS>();
	if (GS)
	{
		GS->AliveCount = Count;
		//For Server
		GS->AliveCount_OnRep();
	}

	return Count;
}

bool ABattleGM::CheckFinish()
{
	if (ReCountAliveCount() == 1)
	{
		return true;
	}
	return false;

}
