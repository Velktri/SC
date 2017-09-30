// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SCSelectable.generated.h"

UCLASS()
class SC_API ASCSelectable : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCSelectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** PlayerState Replication Notification Callback */
	UFUNCTION()
	virtual void OnRep_PlayerState();

	/** Returns Collision Component subobject **/
	class UBoxComponent* GetCollisionComponent() const { return CollisionComponent; }

	UFUNCTION()
	void SetPlayerState(class ASCPlayerState* InPlayerState);

	UFUNCTION()
	void SetPlayerController(class ASCPlayerController* InPlayerController);

	UFUNCTION()
	class ASCPlayerController* GetOwningController();

	UFUNCTION()
	void SetTeamColor();

	class UBehaviorTree* GetBotBehavior();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FString Name;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int32 Cost;

	/** List of units or buildings the actor can create. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TArray<TSubclassOf<ASCSelectable>> BuildableActors;

	/** Building collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selectable")
	class UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selectable")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* TeamColorMID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UTexture* SelectionPortrait;

	UPROPERTY(replicatedUsing = OnRep_PlayerState, VisibleAnywhere, BlueprintReadOnly, Category = "System")
	ASCPlayerState* OwningPlayerState;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "System")
	ASCPlayerController* OwningPlayerController;

	/** Unit's AI movement class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System")
	class UBehaviorTree* BotBehavior;
};
