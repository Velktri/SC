// Fill out your copyright notice in the Description page of Project Settings.

#include "RTS_GameBoundsWall.h"
#include "Components/BoxComponent.h"

AGameBoundsWall::AGameBoundsWall()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	BlockingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BlockingBox->SetupAttachment(RootComponent);
}

class UBoxComponent* AGameBoundsWall::GetBoxComponent()
{
	return BlockingBox;
}

void AGameBoundsWall::SetWallDimensions(FVector InSize)
{
	if (BlockingBox)
	{
		BlockingBox->SetBoxExtent(InSize / 2);
	}
}

void AGameBoundsWall::BeginPlay()
{
	Super::BeginPlay();

	if (BlockingBox)
	{
		BlockingBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		BlockingBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
		BlockingBox->SetCollisionObjectType(ECC_Destructible);
	}
}
