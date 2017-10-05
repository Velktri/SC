// Fill out your copyright notice in the Description page of Project Settings.

#include "SCGameModeBase.h"
#include "SCPlayerController.h"
#include "RTS_PlayerView.h"
#include "RTS_GameBoundsVolume.h"
#include "SCHUD.h"
#include "SCGameStateBase.h"
#include "SCPlayerState.h"
#include "SCSelectable.h"

#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "ConstructorHelpers.h"


ASCGameModeBase::ASCGameModeBase()
{
	PlayerControllerClass = ASCPlayerController::StaticClass();
	DefaultPawnClass = APlayerView::StaticClass();
	GameStateClass = ASCGameStateBase::StaticClass();
	PlayerStateClass = ASCPlayerState::StaticClass();
	HUDClass = ASCHUD::StaticClass();
	SpectatorClass = APlayerView::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBlueprint> HumanBuildingBP(TEXT("Blueprint'/Game/Blueprints/Buildings/HumanStartBuilding.HumanStartBuilding'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> GoblinBuildingBP(TEXT("Blueprint'/Game/Blueprints/Buildings/GoblinStartBuilding.GoblinStartBuilding'"));

	newRace = true;

	if (HumanBuildingBP.Object) { RaceMap.Emplace(ERace::Human, HumanBuildingBP.Object->GeneratedClass); }
	if (GoblinBuildingBP.Object) { RaceMap.Emplace(ERace::Goblin, GoblinBuildingBP.Object->GeneratedClass); }

	PlayerColors.Add(FLinearColor::Gray);
	PlayerColors.Add(FLinearColor::Yellow);
	PlayerColors.Add(FLinearColor::White);
	PlayerColors.Add(FLinearColor::Red);
	PlayerColors.Add(FLinearColor::Blue);
	PlayerColors.Add(FLinearColor::Green);
}

void ASCGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	InitCameraBoundsVolume();	
}

void ASCGameModeBase::InitCameraBoundsVolume() 
{
	UWorld* const world = GetWorld();
	if (world)
	{
		if (!GameBounds)
		{
			for (TActorIterator<AGameBoundsVolume> ActorItr(world); ActorItr; ++ActorItr)
			{
				if (ActorItr->IsA(AGameBoundsVolume::StaticClass()))
				{
					GameBounds = *ActorItr;
					break;
				}
			}
		}
	}
}

void ASCGameModeBase::GenerateStartBuildings(APlayerController* NewPlayer, bool Race)
{
	UWorld* world = GetWorld();
	if (world)
	{
		ASCPlayerController* PlayerController = Cast<ASCPlayerController>(NewPlayer);
		if (PlayerController && PlayerController->IsA(ASCPlayerController::StaticClass()))
		{
			PlayerController->SetGameBounds(GameBounds);
			if (PlayerController->GetPawn())
			{
				FVector StartLocation = PlayerController->GetPawn()->GetActorLocation();
				StartLocation.Z = 10000;

				FVector EndLocation = StartLocation;
				EndLocation.Z = -10000;

				FHitResult Hit(ForceInit);
				if (world->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, COLLISION_GROUND))
				{
					StartLocation = Hit.Location;

					// @TODO: TEMP
					Cast<ASCPlayerState>(PlayerController->PlayerState)->SetRace((Race) ? ERace::Human : ERace::Goblin);
					//

					FActorSpawnParameters SpawnParams;
					ASCSelectable* Building = world->SpawnActor<ASCSelectable>(RaceMap[Cast<ASCPlayerState>(PlayerController->PlayerState)->GetRace()], StartLocation, FRotator(0), SpawnParams);
					if (Building)
					{
						Building->SetPlayerController(PlayerController);
						Building->SetPlayerState(Cast<ASCPlayerState>(PlayerController->PlayerState));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Error: Failed to spawn start building for %s"), *PlayerController->GetName());
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Error: Could not find ground for %s"), *PlayerController->GetName());
				}
			}
		}
	}
}

void ASCGameModeBase::SetPlayerColor(APlayerController* Player)
{
	if (Player && Player->PlayerState)
	{
		ASCPlayerState* PS = Cast<ASCPlayerState>(Player->PlayerState);
		if (PS)
		{
			// @TODO: Add a randomization to the colors
			PS->SetPlayerColor(PlayerColors.Pop(false));
		}
	}
}

APlayerStart* ASCGameModeBase::ChooseRandomSpawn(TArray<APlayerStart*> StartList)
{
	if (StartList.Num() > 0)
	{
		APlayerStart* Start = StartList[FMath::RandRange(0, StartList.Num() - 1)];
		OccupiedSpawns.Add(Start);
		return Start;
	}
	return NULL;
}

void ASCGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	newRace = !newRace;
	SetPlayerColor(NewPlayer);
	GenerateStartBuildings(NewPlayer, newRace);
}

AActor* ASCGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PotentialSpawns;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PotentialSpawn = *It;
		if (!PotentialSpawn->PlayerStartTag.IsEqual("Disabled") && !OccupiedSpawns.Contains(PotentialSpawn))
		{
			PotentialSpawns.Add(PotentialSpawn);
		}
	}

	APlayerStart* ChoosenSpawn = ChooseRandomSpawn(PotentialSpawns);
	return (ChoosenSpawn) ? ChoosenSpawn :  Super::ChoosePlayerStart_Implementation(Player);
}