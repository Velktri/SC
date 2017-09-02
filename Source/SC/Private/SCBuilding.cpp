// Fill out your copyright notice in the Description page of Project Settings.

#include "SCBuilding.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ASCBuilding::ASCBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->CastShadow = true;
	Mesh->SetupAttachment(RootComponent);

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

void ASCBuilding::SetPlayerController(ASCPlayerController* PC) 
{
	if (PC) { OwningPlayerController = PC; }
}

ASCPlayerController* ASCBuilding::GetOwningController() 
{
	return OwningPlayerController;
}

