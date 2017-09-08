// Fill out your copyright notice in the Description page of Project Settings.

#include "SSCActionComands.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSCActionComands::Construct(const FArguments& InArgs)
{
	ChildSlot
	.Padding(FMargin(0, 0, 50, 50))
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Bottom)
	[
		SAssignNew(ActionGrid, SUniformGridPanel)
		.SlotPadding(2)
		.MinDesiredSlotHeight(100)
		.MinDesiredSlotWidth(100)
	];

	int k = 0;
	for (int i = 0; i < Columns; i++)
	{
		for (int j = 0; j < Rows; j++)
		{	
			FString str = FString("Button ");
			str.Append(FString::FromInt(k));
			ActionGrid->AddSlot(i, j)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ButtonColorAndOpacity(FLinearColor::Blue)
				.Text(FText::FromString(str))
				.OnClicked(this, &SSCActionComands::ButtonClick, i, j)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Start Menu"))
					//.Margin(FMargin(5))
				]
			];
			k++;
		}
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SSCActionComands::ButtonClick(int32 ColumnIndex, int32 RowIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Button Pressed at Row %d and Column %d."), RowIndex, ColumnIndex);

	return FReply::Handled();
}