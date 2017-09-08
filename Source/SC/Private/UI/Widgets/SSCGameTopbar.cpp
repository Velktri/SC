// Fill out your copyright notice in the Description page of Project Settings.

#include "SSCGameTopbar.h"
#include "SlateOptMacros.h"
#include "Engine.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSCGameTopbar::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.Padding(10)
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0, 0, 10, 0))
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ButtonColorAndOpacity(FLinearColor::Blue)
				.OnClicked(this, &SSCGameTopbar::StartMenu)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Start Menu"))//.Font(FSlateFontInfo())
					.Margin(FMargin(5))
				]
			]

			+ SHorizontalBox::Slot()
			.Padding(FMargin(0, 0, 10, 0))
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ButtonColorAndOpacity(FLinearColor::Blue)
				.OnClicked(this, &SSCGameTopbar::StartMenu)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Menu 2"))
					.Margin(FMargin(5))
				]
			]

			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ButtonColorAndOpacity(FLinearColor::Blue)
				.OnClicked(this, &SSCGameTopbar::StartMenu)
				//.OnReleased();
				[
					SNew(STextBlock)
					.Text(FText::FromString("Menu 3"))
					.Margin(FMargin(5))
				]
			]
		]

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(FMargin(0, 0, 20, 0))
			[
				SNew(STextBlock).Text(FText::FromString("Resource: ###"))
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(FMargin(0, 0, 20, 0))
			[
				SNew(STextBlock).Text(FText::FromString("Resource: ###"))
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SSCGameTopbar::StartMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Menu Button Pressed."));
	FSlateApplication::Get().SetUserFocusToGameViewport(0);// .SetFocusToGameViewport();
	return FReply::Handled();/*.CaptureMouse(GEngine->GameViewport->GetWindow().ToSharedRef())*/;
}
