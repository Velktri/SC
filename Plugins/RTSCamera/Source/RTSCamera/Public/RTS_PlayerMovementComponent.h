// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "RTS_PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAMERA_API UPlayerMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UPlayerMovementComponent();

protected:
	/** Maximum velocity magnitude allowed for the controlled Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Movement")
	float MaxSpeed;

private:
	bool HasValidData();
};
