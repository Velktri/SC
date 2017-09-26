// Fill out your copyright notice in the Description page of Project Settings.

#include "SCGameModeBase_Lobby.h"
#include "SCHUD_Lobby.h"
#include "SCPlayerController_Lobby.h"


ASCGameModeBase_Lobby::ASCGameModeBase_Lobby()
{
	HUDClass = ASCHUD_Lobby::StaticClass();
	PlayerControllerClass = ASCPlayerController_Lobby::StaticClass();
}
