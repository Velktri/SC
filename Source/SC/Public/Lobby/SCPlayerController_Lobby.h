// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SCPlayerController_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class SC_API ASCPlayerController_Lobby : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASCPlayerController_Lobby();
	
	virtual void BeginPlay() override;
	
};
