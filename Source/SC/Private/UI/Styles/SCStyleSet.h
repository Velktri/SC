#pragma once
#include "SlateBasics.h"
#include "SCGameHUDWidgetStyle.h"

class FSCStyleSet
{

public:
	// Initializes the value of MenuStyleInstance and registers it with the Slate Style Registry.
	static void Initialize();

	// Unregisters the Slate Style Set and then resets the MenuStyleInstance pointer.
	static void Shutdown();

	// Retrieves a reference to the Slate Style pointed to by MenuStyleInstance.
	static const class ISlateStyle& Get()
	{
		return *SCStyleInstance;
	}

	// Retrieves the name of the Style Set.
	static FName GetStyleSetName();

	/** Get the game's default theme */
	static const FSCGameHUDStyle* GetDefaultTheme()
	{
		return &FSCStyleSet::Get().GetWidgetStyle<FSCGameHUDStyle>("InGameHUD");
	}

private:
	// Creates the Style Set.
	static TSharedRef<class FSlateStyleSet> Create();

	// Singleton instance used for our Style Set.
	static TSharedPtr<class FSlateStyleSet> SCStyleInstance;
};