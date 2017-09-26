// Fill out your copyright notice in the Description page of Project Settings.

#include "SCBuilding.h"
#include "Components/StaticMeshComponent.h"
#include "SCPlayerController.h"
#include "SCPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"


// Sets default values
ASCBuilding::ASCBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->CastShadow = true;
	Mesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASCBuilding::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASCBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ASCPlayerController* ASCBuilding::GetOwningController() 
{
	return OwningPlayerController;
}

void ASCBuilding::SetPlayerState(ASCPlayerState* InPlayerState)
{
	OwningPlayerState = InPlayerState;
}

void ASCBuilding::SetPlayerController(ASCPlayerController* InPlayerController)
{
	OwningPlayerController = InPlayerController;
}

void ASCBuilding::SetOwnerColor()
{
	if (Mesh)
	{
		TeamColorMID = Mesh->CreateAndSetMaterialInstanceDynamic(0);
		if (OwningPlayerState)
		{
			TeamColorMID->SetVectorParameterValue("TeamColor", OwningPlayerState->GetPlayerColor());
			Mesh->SetMaterial(0, TeamColorMID);
		}
		else
		{
			FLinearColor color = FLinearColor(0, 0, 0);
			TeamColorMID->SetVectorParameterValue("TeamColor", color);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Could not find Mesh for %s"), *GetName())
	}
}

void ASCBuilding::OnRep_PlayerState()
{
	if (OwningPlayerState)
	{
		SetOwnerColor();
	}
}

void ASCBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASCBuilding, Health);
	DOREPLIFETIME(ASCBuilding, OwningPlayerState);
	DOREPLIFETIME(ASCBuilding, OwningPlayerController);
}