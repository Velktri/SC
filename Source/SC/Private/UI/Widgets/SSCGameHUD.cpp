// Fill out your copyright notice in the Description page of Project Settings.

#include "SSCGameHUD.h"
#include "SCHUD.h"
#include "SlateOptMacros.h"
#include "SSCGameBottomMenu.h"
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
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			/* Top Menu */
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					SAssignNew(TopBarHUD, SSCGameTopbar)
				]
			]
			
			/* Bottom Menu */
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(BottomMenu, SSCGameBottomMenu)
				//SNew(SHorizontalBox)
				//+ SHorizontalBox::Slot()
				//.HAlign(HAlign_Fill)
				//.VAlign(VAlign_Bottom)
				//[
				//	SAssignNew(ActionCommandGrid, SSCActionComands)
				//]
			]
		]
		
		/* Background Overlay */
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Bottom)
		[
			SAssignNew(GameOverlay, SImage)
			.Image(&HUDStyle->OverlayHUD) 
		]
	];

	GameOverlay->SetVisibility(EVisibility::HitTestInvisible);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
