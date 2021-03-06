// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"

#include "SCGameHUDWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct FSCGameHUDStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FSCGameHUDStyle();
	virtual ~FSCGameHUDStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FSCGameHUDStyle& GetDefault();

	/** The actual data describing the widget appearance. */
	UPROPERTY(Category = Appearance, EditAnywhere)
	FSlateBrush OverlayHUD;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class USCGameHUDWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSCGameHUDStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
