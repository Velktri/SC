// Fill out your copyright notice in the Description page of Project Settings.

#include "SCMiniMapSceneCapture2D.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ConstructorHelpers.h"



ASCMiniMapSceneCapture2D::ASCMiniMapSceneCapture2D(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RootComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> MiniMapViewObj(TEXT("/Game/UI/MiniMapView"));
	if (MiniMapViewObj.Succeeded()) { MiniMapView = MiniMapViewObj.Object; }
}

void ASCMiniMapSceneCapture2D::BeginPlay()
{
	Super::BeginPlay();

	if (MiniMapView)
	{
		GetCaptureComponent2D()->TextureTarget = MiniMapView;
		GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;
		GetCaptureComponent2D()->LODDistanceFactor = 0.5f;
	}
}
