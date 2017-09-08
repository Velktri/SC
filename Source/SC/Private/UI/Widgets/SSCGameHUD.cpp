// Fill out your copyright notice in the Description page of Project Settings.

#include "SSCGameHUD.h"
#include "SCHUD.h"
#include "SlateOptMacros.h"
#include "SSCActionComands.h"
#include "SSCGameTopbar.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSCGameHUD::Construct(const FArguments& InArgs)
{
	SCHUD = InArgs._SCHUD;

	ChildSlot
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SAssignNew(TopBarHUD, SSCGameTopbar)
			]
		]

		+ SOverlay::Slot()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(ActionCommandGrid, SSCActionComands)
			]
			
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
