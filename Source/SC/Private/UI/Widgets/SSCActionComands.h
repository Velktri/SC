// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SSCActionComands : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSCActionComands)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedPtr<SUniformGridPanel> ActionGrid;
	int32 Rows = 3;
	int32 Columns = 3;


	FReply ButtonClick(int32 ColumnIndex, int32 RowIndex);
};
