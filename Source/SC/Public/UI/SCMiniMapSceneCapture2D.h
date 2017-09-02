// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "SCMiniMapSceneCapture2D.generated.h"

/**
 * 
 */
UCLASS()
class SC_API ASCMiniMapSceneCapture2D : public ASceneCapture2D
{
	GENERATED_BODY()
	

public:
	ASCMiniMapSceneCapture2D(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	class UTextureRenderTarget2D* MiniMapView;
	
	virtual void BeginPlay() override;
};
