// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SCGameModeBase.generated.h"

class APlayerStart;
/**
 * 
 */
UCLASS()
class SC_API ASCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASCGameModeBase();

	virtual void BeginPlay() override;
	
	/** Override login */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | Bounds")
	class AGameBoundsVolume* GameBounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | Bounds")
	TMap<ERace, TSubclassOf<class ASCBuilding>> RaceMap;

	bool newRace;

private:
	TArray<APlayerStart*> OccupiedSpawns;
	TArray<FLinearColor> PlayerColors;

	/** Sets the Game Bounds */
	void InitCameraBoundsVolume();

	/** Spawns the Starting building for the given player
		based on it's race. */
	void GenerateStartBuildings(APlayerController* NewPlayer, bool Race);

	/** Assign team's/player's colors */
	void SetPlayerColor(APlayerController* Player);

	/** Helper Function */
	APlayerStart* ChooseRandomSpawn(TArray<APlayerStart*> StartList);
};
