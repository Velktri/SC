// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTS_GameBoundsWall.generated.h"

UCLASS()
class RTSCAMERA_API AGameBoundsWall : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameBoundsWall();

	class UBoxComponent* GetBoxComponent();

	/** Change the size of the wall. */
	void SetWallDimensions(FVector InSize);

protected:
	virtual void BeginPlay() override;

private:

	/** Box Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BlockingBox;
};
