// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCTypes.h"
#include "GameFramework/PlayerController.h"
#include "SCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SC_API ASCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASCPlayerController();

	UFUNCTION()
	class APlayerView* GetPlayerView();
	
	UFUNCTION(Category = "SC | Bounds")
	void SetGameBounds(class AGameBoundsVolume* InGameBounds);

	UFUNCTION(Category = "SC | Bounds")
	class AGameBoundsVolume* GetGameBounds();

	UFUNCTION(Category = "SC | Race")
	void SetRace(ERace InRace);

	UFUNCTION(Category = "SC | Race")
	ERace GetRace();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | Bounds")
	class AGameBoundsVolume* GameBounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | Race")
	ERace PlayerRace;

	virtual void BeginPlay() override;
};
