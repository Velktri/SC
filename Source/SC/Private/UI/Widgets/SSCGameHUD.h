// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SC_API SSCGameHUD : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSCGameHUD)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class ASCHUD>, SCHUD)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<class ASCHUD> SCHUD;
	TSharedPtr<class SSCActionComands> ActionCommandGrid;
	TSharedPtr<class SSCGameTopbar> TopBarHUD;
	TSharedPtr<class SSCGameBottomMenu> BottomMenu;
	TSharedPtr<class SImage> GameOverlay;

	//const struct FSCGameHUDStyle& HUDStyle;
};
