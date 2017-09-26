// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCTypes.h"
#include "GameFramework/PlayerState.h"
#include "SCPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SC_API ASCPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASCPlayerState();

	/** Assign the player a color if it has none. */
	void SetPlayerColor(FLinearColor Color);

	FLinearColor GetPlayerColor();

	UFUNCTION(Category = "SC | Race")
	void SetRace(ERace InRace);

	UFUNCTION(Category = "SC | Race")
	ERace GetRace();

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "SC | Race")
	FLinearColor PlayerColor;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "SC | Race")
	ERace PlayerRace;
};
