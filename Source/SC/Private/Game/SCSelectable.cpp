// Fill out your copyright notice in the Description page of Project Settings.

#include "SCSelectable.h"
#include "Net/UnrealNetwork.h"
#include "SCPlayerController.h"
#include "SCPlayerState.h"
#include "SCAIController.h"
#include "SCSelectableMovement.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/DataTable.h"

// Sets default values
ASCSelectable::ASCSelectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UnitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Unit Collision Component"));
	if (UnitCollision)
	{
		/* Collision Settings */
		UnitCollision->SetCollisionObjectType(ECC_Pawn);
		UnitCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UnitCollision->SetCollisionResponseToChannel(ECC_Visibility,		ECR_Block);
		UnitCollision->SetCollisionResponseToChannel(ECC_Camera,			ECR_Ignore);

		UnitCollision->SetCollisionResponseToChannel(COLLISION_SELECTABLE,	ECR_Block);
		UnitCollision->SetCollisionResponseToChannel(COLLISION_GROUND,		ECR_Ignore);

		UnitCollision->SetCollisionResponseToChannel(ECC_WorldStatic,		ECR_Ignore);
		UnitCollision->SetCollisionResponseToChannel(ECC_WorldDynamic,		ECR_Ignore);
		UnitCollision->SetCollisionResponseToChannel(ECC_Pawn,				ECR_Block);
		UnitCollision->SetCollisionResponseToChannel(ECC_PhysicsBody,		ECR_Ignore);
		UnitCollision->SetCollisionResponseToChannel(ECC_Vehicle,			ECR_Ignore);
		UnitCollision->SetCollisionResponseToChannel(ECC_Destructible,		ECR_Ignore);


		RootComponent = UnitCollision;
	}

	BuildingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Building Collision Component"));
	if (BuildingCollision)
	{
		/* Collision Settings */
		BuildingCollision->SetCollisionObjectType(ECC_WorldStatic);
		BuildingCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BuildingCollision->SetCollisionResponseToChannel(ECC_Visibility,		ECR_Block);
		BuildingCollision->SetCollisionResponseToChannel(ECC_Camera,			ECR_Ignore);

		BuildingCollision->SetCollisionResponseToChannel(COLLISION_SELECTABLE,	ECR_Block);
		BuildingCollision->SetCollisionResponseToChannel(COLLISION_GROUND,		ECR_Ignore);

		BuildingCollision->SetCollisionResponseToChannel(ECC_WorldStatic,		ECR_Ignore);
		BuildingCollision->SetCollisionResponseToChannel(ECC_WorldDynamic,		ECR_Ignore);
		BuildingCollision->SetCollisionResponseToChannel(ECC_Pawn,				ECR_Block);
		BuildingCollision->SetCollisionResponseToChannel(ECC_PhysicsBody,		ECR_Ignore);
		BuildingCollision->SetCollisionResponseToChannel(ECC_Vehicle,			ECR_Ignore);
		BuildingCollision->SetCollisionResponseToChannel(ECC_Destructible,		ECR_Ignore);

		BuildingCollision->SetupAttachment(RootComponent);
	}



	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SelectableMesh0"));
	if (Mesh)
	{
		Mesh->CastShadow = true;
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetupAttachment(RootComponent);
	}

	SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Selection Decal"));
	if (SelectionDecal)
	{
		SelectionDecal->AttachTo(Mesh);
		SelectionDecal->AddLocalRotation(FRotator(-90, 0, 0));
		SelectionDecal->DecalSize = FVector(400, 400, 400);
		SelectionDecal->RelativeScale3D = FVector(1, 1, 1);
		SelectionDecal->SetHiddenInGame(true);
	}

	UnitMovement = CreateDefaultSubobject<USCSelectableMovement>(TEXT("Selection Movement"));
	if (UnitMovement)
	{
		UnitMovement->UpdatedComponent = RootComponent;
	}

	bReplicates = true;
	Type = ESelectionType::None;

	AIControllerClass = ASCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ASCSelectable::BeginPlay()
{
	Super::BeginPlay();
	SetData();

	if (Type == ESelectionType::Building) { BuildingCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); }
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

ESelectionType ASCSelectable::GetType()
{
	return Type;
}

void ASCSelectable::Move(FVector MoveLocation, ASCAIController* NavController)
{
	if (!Controller)
	{
		Controller = NavController;
	}

	//NavController->Possess(this);
	//NavController->Move(MoveLocation);

	if (GetController() && GetController()->IsA(ASCAIController::StaticClass()))
	{
		Cast<ASCAIController>(GetController())->Move(MoveLocation);
	}
}

void ASCSelectable::SetData()
{
	if (SelectableDataTable)
	{
		FName name2 = GetClass()->GetFName();
		FSelectableLookupTable* LookUpRow = SelectableDataTable->FindRow<FSelectableLookupTable>(name2, "GENERAL");
		if (LookUpRow)
		{
			/* Set Data */
			Health = LookUpRow->Health;
			Name = LookUpRow->Name;
			Cost = LookUpRow->Cost;
			Type = LookUpRow->Type;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Could not find Data Table for %s"), *GetName())
	}
}
