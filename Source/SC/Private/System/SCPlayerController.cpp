// Fill out your copyright notice in the Description page of Project Settings.

#include "SCPlayerController.h"
#include "RTS_PlayerView.h"
#include "SCHUD.h"
#include "SCSelectable.h"
#include "SCAIController.h"

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

void ASCPlayerController::FilterSelection(TArray<ASCSelectable*> UnfilteredActors)
{
	// @TODO
	bIsClicked = false;
	for (ASCSelectable* actor : UnfilteredActors)
	{
		/*
			if (unit and friendly)
				add to unit selection
			else if (building and friendly)
				add to to building selection
			else if (unit and bad)
				add to bad var
			else if (building and bad)
				add to bad var

			set units to selected array
				if units empty
					set building
						etc....
		*/


		if (actor->IsA(ASCSelectable::StaticClass()))
		{
			ASCSelectable* building = Cast<ASCSelectable>(actor);
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

	if (GetWorld())
	{
		NavController = GetWorld()->SpawnActor<ASCAIController>(ASCAIController::StaticClass());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Nav Controller for Player \"%s\" failed to spawn."), *this->GetName());
	}
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
	FVector2D MousePosition = FVector2D::ZeroVector;
	if (!ClickedUI() && GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		ClickedMousePosition = MousePosition;
		bIsClicked = true;
	}
}

void ASCPlayerController::StopBoxSelection()
{
	FVector2D MousePosition = FVector2D::ZeroVector;
	if (GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		ASCHUD* HUD = Cast<ASCHUD>(GetHUD());
		if (HUD)
		{
			HUD->QuerySelection();
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
