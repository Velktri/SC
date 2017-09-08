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
	void SetPlayerController(class ASCPlayerController* PC);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Component")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Component")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Resourses")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Resourses")
	int32 Cost;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Component")
	//TArray<TSubclassOf<class ASCUnit>> BuildableUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Materials")
	class UTexture* SelectionPortrait;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | System")
	class ASCPlayerController* OwningPlayerController;

	UFUNCTION()
	class ASCPlayerController* GetOwningController();
};
