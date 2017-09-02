// Fill out your copyright notice in the Description page of Project Settings.

#include "SCPlayerController.h"
#include "RTS_PlayerView.h"



ASCPlayerController::ASCPlayerController() 
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	PlayerRace = ERace::None;
}

APlayerView* ASCPlayerController::GetPlayerView() 
{
	return Cast<APlayerView>(GetPawn());
}

void ASCPlayerController::SetGameBounds(AGameBoundsVolume* InGameBounds)
{
	GameBounds = InGameBounds;
}

class AGameBoundsVolume* ASCPlayerController::GetGameBounds()
{
	return GameBounds;
}

void ASCPlayerController::SetRace(ERace InRace) 
{
	if (PlayerRace == ERace::None)
	{
		PlayerRace = InRace;
	}
}

ERace ASCPlayerController::GetRace() 
{
	return PlayerRace;
}

void ASCPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetRace(ERace::Human);
}
