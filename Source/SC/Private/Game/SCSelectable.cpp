// Fill out your copyright notice in the Description page of Project Settings.

#include "SCSelectable.h"
#include "Net/UnrealNetwork.h"
#include "SCPlayerController.h"
#include "SCPlayerState.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BehaviorTree/BehaviorTree.h"

// Sets default values
ASCSelectable::ASCSelectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Building Collision Component"));
	if (CollisionComponent)
	{
		CollisionComponent->SetupAttachment(RootComponent);
	}

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SelectableMesh0"));
	if (Mesh)
	{
		Mesh->CastShadow = true;
		Mesh->SetupAttachment(RootComponent);
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASCSelectable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASCSelectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCSelectable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASCSelectable, Health);
	DOREPLIFETIME(ASCSelectable, OwningPlayerState);
	DOREPLIFETIME(ASCSelectable, OwningPlayerController);
}

void ASCSelectable::OnRep_PlayerState()
{
	if (OwningPlayerState)
	{
		SetTeamColor();
	}
}

ASCPlayerController* ASCSelectable::GetOwningController()
{
	return OwningPlayerController;
}

void ASCSelectable::SetPlayerState(ASCPlayerState* InPlayerState)
{
	OwningPlayerState = InPlayerState;
}

void ASCSelectable::SetPlayerController(ASCPlayerController* InPlayerController)
{
	OwningPlayerController = InPlayerController;
}

void ASCSelectable::SetTeamColor()
{
	if (Mesh)
	{
		TeamColorMID = Mesh->CreateAndSetMaterialInstanceDynamic(0);
		if (TeamColorMID)
		{
			if (OwningPlayerState)
			{
				TeamColorMID->SetVectorParameterValue("TeamColor", OwningPlayerState->GetPlayerColor());
				Mesh->SetMaterial(0, TeamColorMID);
			}
			else
			{
				TeamColorMID->SetVectorParameterValue("TeamColor", FLinearColor(0, 0, 0));
				UE_LOG(LogTemp, Error, TEXT("Error: Failed to find PlayerState for %s"), *GetName())
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Error: Failed to find Material for %s"), *GetName())
		}
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Could not find Mesh for %s"), *GetName())
	}
}

UBehaviorTree* ASCSelectable::GetBotBehavior()
{
	return BotBehavior;
}