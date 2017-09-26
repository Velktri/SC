// Fill out your copyright notice in the Description page of Project Settings.

#include "SCPlayerController_Lobby.h"

ASCPlayerController_Lobby::ASCPlayerController_Lobby()
{
	bShowMouseCursor = true;
}

void ASCPlayerController_Lobby::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI inputType;
	inputType.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	inputType.SetHideCursorDuringCapture(false);
	SetInputMode(inputType);
}
