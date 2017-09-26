// Fill out your copyright notice in the Description page of Project Settings.

#include "SCPlayerController.h"
#include "RTS_PlayerView.h"
#include "SCBuilding.h"
#include "SCHUD.h"
#include "Engine.h"
#include "Engine/GameViewportClient.h"

ASCPlayerController::ASCPlayerController() 
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	PlayerCameraManagerClass = NULL;
	bIsClicked = false;
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

bool ASCPlayerController::IsClicked()
{
	return bIsClicked;
}

FVector2D ASCPlayerController::GetClickAnchor()
{
	return ClickedMousePosition;
}

void ASCPlayerController::FilterSelection(TArray<AActor*> UnfilteredActors)
{
	// @TODO
	bIsClicked = false;
	for (AActor* actor : UnfilteredActors)
	{
		if (actor->IsA(ASCBuilding::StaticClass()))
		{
			ASCBuilding* building = Cast<ASCBuilding>(actor);
			UE_LOG(LogTemp, Warning, TEXT("Found a building actor: %s"), *building->GetName());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Scanned %d Actors."), UnfilteredActors.Num());
}

void ASCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI inputType;
	inputType.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	inputType.SetHideCursorDuringCapture(false);
	SetInputMode(inputType);
}

void ASCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ASCPlayerController::StartBoxSelection);
	InputComponent->BindAction("LeftClick", IE_Released, this, &ASCPlayerController::StopBoxSelection);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &ASCPlayerController::RightClick);
}

void ASCPlayerController::StartBoxSelection()
{
	FVector2D MousePosition;
	if (!ClickedUI() && GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		ClickedMousePosition = MousePosition;
		bIsClicked = true;
	}
}

void ASCPlayerController::StopBoxSelection()
{
	/*	@TODO:
		Get mouse position
		get all actors in selection
		filter actors based on what was found
	*/

	if (GetLocalPlayer() && GetLocalPlayer()->ViewportClient)
	{
		UGameViewportClient* GameViewport = GetLocalPlayer()->ViewportClient;
		FVector2D MousePosition = FVector2D(0, 0);

		check(GameViewport);
		if (GameViewport->IsFocused(GameViewport->Viewport) && GameViewport->GetMousePosition(MousePosition))
		{
			ASCHUD* HUD = Cast<ASCHUD>(GetHUD());
			if (HUD)
			{
				HUD->QuerySelection();
			}
		}
	}
}

void ASCPlayerController::RightClick()
{
	if (!ClickedUI())
	{
		/*
			@TODO:
			Get click point
			send the point to the AI controller
		*/
	}
}

bool ASCPlayerController::ClickedUI()
{
	// @TODO: Implement
	return false;
}
