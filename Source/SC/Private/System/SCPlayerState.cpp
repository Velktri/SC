// Fill out your copyright notice in the Description page of Project Settings.

#include "SCPlayerState.h"
#include "Net/UnrealNetwork.h"

ASCPlayerState::ASCPlayerState()
{
	PlayerColor = FLinearColor::Black;
	PlayerRace = ERace::None;
}

void ASCPlayerState::SetPlayerColor(FLinearColor Color)
{
	if (PlayerColor == FLinearColor::Black)
	{
		PlayerColor = Color;
	}
}

FLinearColor ASCPlayerState::GetPlayerColor()
{
	return PlayerColor;
}

void ASCPlayerState::SetRace(ERace InRace)
{
	PlayerRace = InRace;
}

ERace ASCPlayerState::GetRace()
{
	return PlayerRace;
}

void ASCPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCPlayerState, PlayerRace);
	DOREPLIFETIME(ASCPlayerState, PlayerColor);

}