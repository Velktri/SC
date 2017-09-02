// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "CameraBoundsVolume.generated.h"

/**
 * 
 */
UCLASS()
class SC_API ACameraBoundsVolume : public AVolume
{
	GENERATED_BODY()

public:
	ACameraBoundsVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FBox WorldBounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform ComponentTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector AxisX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector AxisY;

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ASCMiniMapSceneCapture2D* MiniMapCamera;
};
