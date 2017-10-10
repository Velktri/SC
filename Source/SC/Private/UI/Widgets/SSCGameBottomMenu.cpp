// Fill out your copyright notice in the Description page of Project Settings.

#include "SSCGameBottomMenu.h"
#include "SSCGameSelectionSet.h"
#include "SSCActionComands.h"
#include "SSCGameMinimap.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSCGameBottomMenu::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SHorizontalBox)

		/* Minimap */
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Bottom)
		[
			SAssignNew(Minimap, SSCGameMinimap)
		]

		/* Selection Box */
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(20, 0, 0, 0)
		[
			SAssignNew(Selections, SSCGameSelectionSet)
		]

		/* Action Commands */
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(ActionGrid, SSCActionComands)
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
