// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SSCGameBottomMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSCGameBottomMenu)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedPtr<class SSCGameMinimap> Minimap;
	TSharedPtr<class SSCGameSelectionSet> Selections;
	TSharedPtr<class SSCActionComands> ActionGrid;
};
