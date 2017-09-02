// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraBoundsVolume.h"
#include "Components/BrushComponent.h"
#include "SCMiniMapSceneCapture2D.h"
#include "Engine/World.h"

ACameraBoundsVolume::ACameraBoundsVolume() 
{
	BrushColor = FColor::Green;
}

void ACameraBoundsVolume::BeginPlay() 
{
	UBrushComponent* BrushComp = GetBrushComponent();
	if (BrushComp)
	{
		ComponentTransform = BrushComp->GetComponentTransform();
		AxisX = ComponentTransform.GetScaledAxis(EAxis::X).GetUnsafeNormal();
		AxisY = ComponentTransform.GetScaledAxis(EAxis::Y).GetUnsafeNormal();

		WorldBounds = BrushComp->CalcBounds(BrushComp->GetComponentTransform()).GetBox();

		UWorld* World = GetWorld();
		if (World)
		{
			FVector CameraLocation = WorldBounds.GetCenter();
			CameraLocation.Z = (WorldBounds.GetExtent().X > WorldBounds.GetExtent().Y) ? WorldBounds.GetExtent().X : WorldBounds.GetExtent().Y;

			MiniMapCamera = World->SpawnActor<ASCMiniMapSceneCapture2D>(CameraLocation, GetActorRotation());
		}
	}
}