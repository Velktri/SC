// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTS_PlayerView.generated.h"

UCLASS()
class RTSCAMERA_API APlayerView : public APawn
{
	GENERATED_BODY()

public:
	APlayerView();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	/** Returns Collision Component subobject **/
	class USphereComponent* GetCollisionComponent() const { return CollisionComponent; }

	/** Returns Camera Component subobject **/
	class UCameraComponent* GetCameraComponent() const { return PlayerCamera; }

	/** Name of the CollisionComponent. */
	static FName CollisionComponentName;

protected:
	/** Camera's Field of View */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float FieldOfView;

	/** Pitch used by the camera to look down at the level. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float CameraRotationPitch;

	/** Minimum distance the Camera can be from the Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float MinCameraDistance;

	/** Maximum distance the Camera can be from the Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float MaxCameraDistance;

	/** Distance the Camera is from the Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float CameraDistance;

	/** Changes the rate at which the camera zooms in or out. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float CameraDistanceModifer;

	/** Should the camera move? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	bool bCanMoveCamera;

	/** Changes the rate at which the mouse scrolls the camera. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float CameraMouseSpeedModifier;

	/** Changes the rate at which key presses scroll the camera. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float CameraKeyboardSpeedModifier;

	/** Vector the Pawn uses to move left or right. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	FVector Transform_X;

	/** Vector the Pawn uses to move up or down. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	FVector Transform_Y;

	/** The amount of pixels between the mouse and the edge of the screen.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float CameraScrollBoundary;

	/** Camera's Base Yaw Rotation.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin | Camera")
	float CameraYawRotation;

	/** Move along the Transform X vector by the given speed. */
	UFUNCTION(BlueprintCallable, Category = "Plugin | Camera")
	void MoveVertical(float Speed);

	/** Move along the Transform Y vector by the given speed. */
	UFUNCTION(BlueprintCallable, Category = "Plugin | Camera")
	void MoveHorizontal(float Speed);

	/** Decrease the camera distance. */
	UFUNCTION(BlueprintCallable, Category = "Plugin | Camera")
	void ZoomIn();

	/** Increase the camera distance. */
	UFUNCTION(BlueprintCallable, Category = "Plugin | Camera")
	void ZoomOut();

	/** Update Camera's FoV, Pitch, and Camera distance. */
	UFUNCTION(BlueprintCallable, Category = "Plugin | Camera")
	void UpdateCamera();

	/** Rotate Camera via its Yaw */
	UFUNCTION(BlueprintCallable, Category = "Plugin | Camera")
	void RotateCameraYaw(float InAmount);

	/** Rotate Camera via its Pitch */
	UFUNCTION(BlueprintCallable, Category = "Plugin | Camera")
	void RotateCameraPitch(float InAmount);

	/** Rotate Camera via its Yaw */
	UFUNCTION(BlueprintCallable, Category = "Plugin | Camera")
	void ResetCameraRotation();

private:

	void CalculateCameraDirection(FVector Direction, float Distance);

	/** Checks if mouse is at the edge of the screen */
	bool bCanScroll(float InBound);

	/** DefaultPawn collision component */
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;
	
	/** Camera Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plugin | Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

	/** Movement Component */
	UPROPERTY(Category = "Plugin | Movement", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPlayerMovementComponent* PlayerMovement;

	/** Camera Arm Component */
	UPROPERTY(Category = "Plugin | Movement", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CameraBoom;
};
