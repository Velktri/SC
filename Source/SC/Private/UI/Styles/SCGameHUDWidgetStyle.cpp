// Fill out your copyright notice in the Description page of Project Settings.

#include "SCGameHUDWidgetStyle.h"


FSCGameHUDStyle::FSCGameHUDStyle()
{
}

FSCGameHUDStyle::~FSCGameHUDStyle()
{
}

const FName FSCGameHUDStyle::TypeName(TEXT("SCGameHUDStyle"));

const FSCGameHUDStyle& FSCGameHUDStyle::GetDefault()
{
	static FSCGameHUDStyle Default;
	return Default;
}

void FSCGameHUDStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
	OutBrushes.Add(&OverlayHUD);
}

