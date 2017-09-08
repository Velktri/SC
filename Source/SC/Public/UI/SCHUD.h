// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SCHUD.generated.h"

/**
 * 
 */
UCLASS()
class SC_API ASCHUD : public AHUD
{
	GENERATED_BODY()
	

	ASCHUD();

	// Initializes the Slate UI and adds it as widget content to the game viewport.
	virtual void PostInitializeComponents() override;

	// Reference to the Main Menu Slate UI.
	TSharedPtr<class SSCGameHUD> InGameHUD;

};
