// Fill out your copyright notice in the Description page of Project Settings.

#include "SCHUD.h"
#include "Widgets/SSCGameHUD.h"
#include "SCPlayerController.h"
#include "Engine.h"

ASCHUD::ASCHUD()
{
	bQuery = false;
}

void ASCHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SAssignNew(InGameHUD, SSCGameHUD).SCHUD(this);

	if (GEngine->IsValidLowLevel())
	{
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(InGameHUD.ToSharedRef()));
	}
}

TSharedPtr<class SSCGameHUD> ASCHUD::GetGameHUD()
{
	return InGameHUD;
}

void ASCHUD::QuerySelection()
{
	bQuery = true;
}

void ASCHUD::DrawHUD()
{
	Super::DrawHUD();
	if (!Canvas) { return; }

	ASCPlayerController* PC = Cast<ASCPlayerController>(GetOwningPlayerController());
	if (PC)
	{
		PC->GetMousePosition(CurrentMouseLocation.X, CurrentMouseLocation.Y);
		/* Draws the selection box */
		if (PC->IsClicked())
		{
			MouseAnchor = PC->GetClickAnchor();
			DrawSelectionBox();
			if (bQuery)
			{
				TArray<AActor*> SelectedActors;
				GetActorsInSelectionRectangle<AActor>(MouseAnchor, CurrentMouseLocation, SelectedActors, true, false);
				PC->FilterSelection(SelectedActors);
				bQuery = false;
			}
		}
	}
}

void ASCHUD::DrawSelectionBox()
{
	DrawRect(FLinearColor(0, 0, 1, 0.4f),
			 MouseAnchor.X,
			 MouseAnchor.Y,
			 CurrentMouseLocation.X - MouseAnchor.X,
			 CurrentMouseLocation.Y - MouseAnchor.Y);
}
