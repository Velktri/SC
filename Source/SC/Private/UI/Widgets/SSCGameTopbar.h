// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SSCGameTopbar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSCGameTopbar)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	FReply StartMenu();
};
