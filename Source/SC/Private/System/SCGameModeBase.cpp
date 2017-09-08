// Fill out your copyright notice in the Description page of Project Settings.

#include "SCGameModeBase.h"
#include "SCPlayerController.h"
#include "RTS_PlayerView.h"
#include "RTS_GameBoundsVolume.h"
#include "SCBuilding.h"
#include "SCHUD.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "ConstructorHelpers.h"


ASCGameModeBase::ASCGameModeBase()
{
	PlayerControllerClass = ASCPlayerController::StaticClass();
	DefaultPawnClass = APlayerView::StaticClass();
	HUDClass = ASCHUD::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBlueprint> HumanBuildingBP(TEXT("Blueprint'/Game/Blueprints/Buildings/HumanStartBuilding.HumanStartBuilding'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> GoblinBuildingBP(TEXT("Blueprint'/Game/Blueprints/Buildings/GoblinStartBuilding.GoblinStartBuilding'"));

	if (HumanBuildingBP.Object) { RaceMap.Emplace(ERace::Human, HumanBuildingBP.Object->GeneratedClass); }
	if (GoblinBuildingBP.Object) { RaceMap.Emplace(ERace::Goblin, GoblinBuildingBP.Object->GeneratedClass); }
}

void ASCGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const world = GetWorld();
	if (world)
	{
		InitCameraBoundsVolume(world);
		GenerateStartBuildings(world);
	}
}

void ASCGameModeBase::InitCameraBoundsVolume(UWorld* World) 
{
	if (!GameBounds)
	{
		for (TActorIterator<AGameBoundsVolume> ActorItr(World); ActorItr; ++ActorItr)
		{
			if (ActorItr->IsA(AGameBoundsVolume::StaticClass()))
			{
				GameBounds = *ActorItr;
				break;
			}
		}
	}
}

void ASCGameModeBase::GenerateStartBuildings(UWorld* World) 
{
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator) 
	{
		ASCPlayerController* PlayerController = Cast<ASCPlayerController>(Iterator->Get());
		if (PlayerController && PlayerController->IsA(ASCPlayerController::StaticClass())) 
		{
			PlayerController->SetGameBounds(GameBounds);
			AActor* PlayerStart = FindPlayerStart(PlayerControllerClass.GetDefaultObject(), "PlayerStart");
			if (PlayerStart) 
			{
				FVector Location = PlayerStart->GetActorLocation();
				FVector EndLocation = Location + FVector(Location.X, Location.Y, Location.Z - 10000);
				FHitResult Hit(ForceInit);
				World->LineTraceSingleByChannel(Hit, Location, EndLocation, ECollisionChannel::ECC_Visibility);
				Location = Hit.Location;

				FActorSpawnParameters SpawnParams;
				ASCBuilding* Building = World->SpawnActor<ASCBuilding>(RaceMap[PlayerController->GetRace()], Location, FRotator(0), SpawnParams);
				Building->SetPlayerController(PlayerController);
			}
		}
	}
}
