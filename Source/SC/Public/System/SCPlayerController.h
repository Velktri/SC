// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCTypes.h"
#include "GameFramework/PlayerController.h"
#include "SCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SC_API ASCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASCPlayerController();

	UFUNCTION()
	class APlayerView* GetPlayerView();
	
	UFUNCTION(Category = "SC | Bounds")
	void SetGameBounds(class AGameBoundsVolume* InGameBounds);

	UFUNCTION(Category = "SC | Bounds")
	class AGameBoundsVolume* GetGameBounds();

	UFUNCTION()
	bool IsClicked();

	UFUNCTION()
	FVector2D GetClickAnchor();

	UFUNCTION()
	void FilterSelection(TArray<ASCSelectable*> UnfilteredActors);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | Bounds")
	class AGameBoundsVolume* GameBounds;

	/** List of Current Selection */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Selection")
	TArray<ASCSelectable*> CurrentSelection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | System")
	class ASCAIController* NavController;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void StartBoxSelection();
	void StopBoxSelection();
	void RightClick();

private:
	FVector2D ClickedMousePosition;
	bool bIsClicked;
	bool ClickedUI();
	void AddFilter(TMap<ESelectionType, TArray<ASCSelectable*>>& Filter, ESelectionType type, ASCSelectable* actor);
};
