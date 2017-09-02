// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SCGameModeBase.generated.h"

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
	
	void InitCameraBoundsVolume(UWorld* World);
	void GenerateStartBuildings(UWorld* World);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | Bounds")
	class AGameBoundsVolume* GameBounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | Bounds")
	TMap<ERace, TSubclassOf<class ASCBuilding>> RaceMap;
};
