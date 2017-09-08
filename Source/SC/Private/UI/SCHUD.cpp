// Fill out your copyright notice in the Description page of Project Settings.

#include "SCHUD.h"
#include "Widgets/SSCGameHUD.h"
#include "Engine.h"



ASCHUD::ASCHUD()
{

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
