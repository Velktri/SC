#pragma once
#include "SCTypes.generated.h"

UENUM(BlueprintType)
enum class ERace : uint8
{
	None		UMETA(DisplayName = "None"),
	Random		UMETA(DIsplayName = "Random"),
	Human		UMETA(DisplayName = "Human"),
	Goblin		UMETA(DisplayName = "Goblin"),
};

UENUM(BlueprintType)
enum class ETeamColor : uint8 
{
	None		UMETA(DisplayName = "None"),
	Red			UMETA(DisplayName = "Red"),
	Green		UMETA(DisplayName = "Green"),
	Blue		UMETA(DisplayName = "Blue"),
	Teal		UMETA(DisplayName = "Teal"),
	White		UMETA(DisplayName = "White"),
	Orange		UMETA(DisplayName = "Orange"),
};

UENUM(BlueprintType)
enum class ESelectionType : uint8
{
	None			UMETA(DisplayName = "None"),
	UnSelectable	UMETA(DisplayName = "UnSelectable"),
	Unit			UMETA(DisplayName = "Unit"),
	Building		UMETA(DisplayName = "Building"),
};

#define COLLISION_GROUND			ECC_GameTraceChannel1
#define COLLISION_SELECTABLE		ECC_GameTraceChannel2