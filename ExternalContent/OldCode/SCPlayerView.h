// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SCPlayerView.generated.h"

UCLASS()
class SC_API ASCPlayerView : public APawn
{
	GENERATED_BODY()

public:
	ASCPlayerView();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | Camera")
	class UCameraComponent* PlayerCamera;

//#if UE_BUILD_DEBUG
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SC | Camera")
	class UPointLightComponent* DebugLight;
//#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Camera")
	float FieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Camera")
	FRotator CameraRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Camera")
	float CameraDistance;

	/** Should the camera move? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Camera")
	bool bCanMoveCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Camera")
	float CameraSpeedModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Camera")
	FVector TransformX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SC | Camera")
	FVector TransformY;

	UPROPERTY(Category = "SC | Movement", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPlayerMovement* PlayerMovement;

	UFUNCTION()
	void MoveVertical(float Speed);

	UFUNCTION()
	void MoveHorizontal(float Speed);

private:
	void SetupCamera();

	void CalculateCameraDirection(FVector Direction, float Distance);

	bool bCanScroll(float InBound);

	class ASCPlayerController* GetSCPlayerContorller();

	FVector2D MousePosition;

	/** The amount of pixels between the mouse and the edge of the screen.*/
	float CameraScrollBoundary;
	
};
