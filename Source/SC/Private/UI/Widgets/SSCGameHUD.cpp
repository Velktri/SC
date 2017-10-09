// Fill out your copyright notice in the Description page of Project Settings.

#include "SSCGameHUD.h"
#include "SCHUD.h"
#include "SlateOptMacros.h"
#include "SSCActionComands.h"
#include "SSCGameTopbar.h"
#include "SCStyleSet.h"
#include "SCGameHUDWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSCGameHUD::Construct(const FArguments& InArgs)
{
	SCHUD = InArgs._SCHUD;
	const FSCGameHUDStyle* HUDStyle = FSCStyleSet::GetDefaultTheme();

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					SAssignNew(TopBarHUD, SSCGameTopbar)
				]
			]

			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Bottom)
				[
					SAssignNew(ActionCommandGrid, SSCActionComands)
				]
			]
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Bottom)
		[
			SNew(SImage)
			.Image(&HUDStyle->OverlayHUD)
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
