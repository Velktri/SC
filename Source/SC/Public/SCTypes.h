#pragma once
#include "SCTypes.generated.h"

UENUM(BlueprintType)
enum class ERace : uint8 {
	None		UMETA(DisplayName = "None"),
	Human		UMETA(DisplayName = "Human"),
	Goblin		UMETA(DisplayName = "Goblin"),
};