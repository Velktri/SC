// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCBuilding.generated.h"

UCLASS()
class SC_API ASCBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	class ASCPlayerController* GetOwningController();

	UFUNCTION()
	void SetPlayerState(class ASCPlayerState* InPlayerState);

	UFUNCTION()
	void SetPlayerController(class ASCPlayerController* InPlayerController);

	void SetOwnerColor();

	/** PlayerState Replication Notification Callback */
	UFUNCTION()
	virtual void OnRep_PlayerState();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Component")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Component")
	UStaticMeshComponent* Mesh;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SC | Resourses")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Resourses")
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Materials")
	UMaterialInstanceDynamic* TeamColorMID;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Component")
	//TArray<TSubclassOf<class ASCUnit>> BuildableUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Materials")
	class UTexture* SelectionPortrait;

	UPROPERTY(replicatedUsing=OnRep_PlayerState, EditAnywhere, BlueprintReadWrite, Category = "SC | Component")
	ASCPlayerState* OwningPlayerState;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SC | Component")
	ASCPlayerController* OwningPlayerController;
};
