// Fill out your copyright notice in the Description page of Project Settings.

#include "SSCGameSelectionSet.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSCGameSelectionSet::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		[
			SNew(SGridPanel)
			+SGridPanel::Slot(0, 1)
			[
				SNew(SButton)
				.ButtonColorAndOpacity(FLinearColor::Green)
			]
		]

		+SVerticalBox::Slot()
		[
			SNew(SGridPanel)
			+ SGridPanel::Slot(0, 1)
			[
				SNew(SButton)
				.ButtonColorAndOpacity(FLinearColor::Green)
			]
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
