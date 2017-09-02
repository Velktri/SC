// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "RTS_GameBoundsVolume.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAMERA_API AGameBoundsVolume : public AVolume
{
	GENERATED_BODY()
	
public:
	AGameBoundsVolume();

	virtual void BeginPlay() override;

	/** Reference to the world box dimensions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FBox WorldBounds;

	/** Reference to the Volume's X Axis */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector AxisX;

	/** Reference to the Volume's Y Axis */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector AxisY;

	/** Size of the Volume in the X dimension */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Length_X;

	/** Size of the Volume in the Y dimension */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Length_Y;

	/** Desired sizes for the Game Wall actors that are spawned. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float WallSize;

	/** List of Game Bound Wall created in BeginPlay. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class AGameBoundsWall*> WallList;

	// @TODO Add ability to change Volume and Wall Bounds during runtime.
};
