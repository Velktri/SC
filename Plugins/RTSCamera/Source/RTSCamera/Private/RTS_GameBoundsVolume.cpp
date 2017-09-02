// Fill out your copyright notice in the Description page of Project Settings.

#include "RTS_GameBoundsVolume.h"
#include "Components/BrushComponent.h"
#include "Components/BoxComponent.h"
#include "RTS_GameBoundsWall.h"
#include "Engine/World.h"
#include "Math/Box.h"

AGameBoundsVolume::AGameBoundsVolume() 
{
	BrushColor = FColor::Green;
	WallSize = 1000;
}

void AGameBoundsVolume::BeginPlay() 
{
	UBrushComponent* BrushComp = GetBrushComponent();
	if (BrushComp) {
		BrushComp->Mobility = EComponentMobility::Movable;
		FTransform ComponentTransform = BrushComp->GetComponentTransform();

		FQuat rotation = ComponentTransform.GetRotation();
		SetActorRelativeRotation(FRotator(0));
		FVector Size = BrushComp->CalcBounds(BrushComp->GetComponentTransform()).GetBox().GetSize();
		Length_X = Size.X;
		Length_Y = Size.Y;
		SetActorRotation(rotation);
		BrushComp->Mobility = EComponentMobility::Static;

		AxisX = ComponentTransform.GetScaledAxis(EAxis::X).GetUnsafeNormal();
		AxisY = ComponentTransform.GetScaledAxis(EAxis::Y).GetUnsafeNormal();

		WorldBounds = BrushComp->CalcBounds(BrushComp->GetComponentTransform()).GetBox();

		// spawn blocking volumes at the edges of each side
		UWorld* World = GetWorld();
		if (World) 
		{
			for (int i = 0; i < 4; i++)
			{
				FRotator VolumeRotation = GetActorRotation() + FRotator(0, (i * 90), 0);
				AGameBoundsWall* Wall = World->SpawnActor<AGameBoundsWall>(WorldBounds.GetCenter(), VolumeRotation);

				if (Wall)
				{
					if (i % 2 == 0)
					{
						Wall->SetWallDimensions(FVector(Length_X, WallSize, WallSize));
						Wall->AddActorLocalOffset(FVector(0, (Length_Y + WallSize) / 2, 0));
					}
					else
					{
						Wall->SetWallDimensions(FVector(Length_Y + (WallSize * 2), WallSize, WallSize));
						Wall->AddActorLocalOffset(FVector(0, (Length_X + WallSize) / 2, 0));
					}

					WallList.Add(Wall);
				}
			}
		}
	}
}