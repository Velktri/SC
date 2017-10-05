// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h" //FloatingPawnMovement.h" 
#include "SCSelectableMovement.generated.h"

///** Data about the floor for walking movement, used by CharacterMovementComponent. */
//USTRUCT(BlueprintType)
//struct ENGINE_API FSCFindFloorResult
//{
//	GENERATED_USTRUCT_BODY()
//
//	/**
//	* True if there was a blocking hit in the floor test that was NOT in initial penetration.
//	* The HitResult can give more info about other circumstances.
//	*/
//	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=CharacterFloor)
//	uint32 bBlockingHit:1;
//
//	/** True if the hit found a valid walkable floor. */
//	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=CharacterFloor)
//	uint32 bWalkableFloor:1;
//
//	/** True if the hit found a valid walkable floor using a line trace (rather than a sweep test, which happens when the sweep test fails to yield a walkable surface). */
//	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=CharacterFloor)
//	uint32 bLineTrace:1;
//
//	/** The distance to the floor, computed from the swept capsule trace. */
//	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=CharacterFloor)
//	float FloorDist;
//
//	/** The distance to the floor, computed from the trace. Only valid if bLineTrace is true. */
//	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=CharacterFloor)
//	float LineDist;
//
//	/** Hit result of the test that found a floor. Includes more specific data about the point of impact and surface normal at that point. */
//	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=CharacterFloor)
//	FHitResult HitResult;
//
//public:
//
//	FSCFindFloorResult()
//		: bBlockingHit(false)
//		, bWalkableFloor(false)
//		, bLineTrace(false)
//		, FloorDist(0.f)
//		, LineDist(0.f)
//		, HitResult(1.f)
//	{
//	}
//
//	/** Returns true if the floor result hit a walkable surface. */
//	bool IsWalkableFloor() const
//	{
//		return bBlockingHit && bWalkableFloor;
//	}
//
//	void Clear()
//	{
//		bBlockingHit = false;
//		bWalkableFloor = false;
//		bLineTrace = false;
//		FloorDist = 0.f;
//		LineDist = 0.f;
//		HitResult.Reset(1.f, false);
//	}
//
//	/** Gets the distance to floor, either LineDist or FloorDist. */
//	float GetDistanceToFloor() const
//	{
//		// When the floor distance is set using SetFromSweep, the LineDist value will be reset.
//		// However, when SetLineFromTrace is used, there's no guarantee that FloorDist is set.
//		return bLineTrace ? LineDist : FloorDist;
//	}
//
//	void SetFromSweep(const FHitResult& InHit, const float InSweepFloorDist, const bool bIsWalkableFloor);
//	void SetFromLineTrace(const FHitResult& InHit, const float InSweepFloorDist, const float InLineDist, const bool bIsWalkableFloor);
//};

/**
 * 
 */
UCLASS()
class SC_API USCSelectableMovement : public /*UFloatingPawnMovement*/ UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	USCSelectableMovement();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	/**
	*	Apply gravity while the character is actively jumping (e.g. holding the jump key).
	*	Helps remove frame-rate dependent jump height, but may alter base jump height.
	*/
	UPROPERTY(Category="SCPawnMovement", EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	uint32 bApplyGravityWhileJumping:1;

	/** Custom gravity scale. Gravity is multiplied by this amount for the character. */
	UPROPERTY(Category="SCPawnMovement", EditAnywhere, BlueprintReadWrite)
	float GravityScale;

	/** Maximum height character can step up */
	UPROPERTY(Category="SCPawnMovement", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float MaxStepHeight;

private:
	/**
	* Max angle in degrees of a walkable surface. Any greater than this and it is too steep to be walkable.
	*/
	UPROPERTY(Category="SCPawnMovement", EditAnywhere, meta=(ClampMin="0.0", ClampMax="90.0", UIMin = "0.0", UIMax = "90.0"))
	float WalkableFloorAngle;

	/**
	* Minimum Z value for floor normal. If less, not a walkable surface. Computed from WalkableFloorAngle.
	*/
	UPROPERTY(Category="SCPawnMovement", VisibleAnywhere)
	float WalkableFloorZ;

public:
	/**
	* Actor's current movement mode (walking, falling, etc).
	*    - walking:  Walking on a surface, under the effects of friction, and able to "step up" barriers. Vertical velocity is zero.
	*    - falling:  Falling under the effects of gravity, after jumping or walking off the edge of a surface.
	*    - flying:   Flying, ignoring the effects of gravity.
	*    - swimming: Swimming through a fluid volume, under the effects of gravity and buoyancy.
	*    - custom:   User-defined custom movement mode, including many possible sub-modes.
	* This is automatically replicated through the Character owner and for client-server movement functions.
	* @see SetMovementMode(), CustomMovementMode
	*/
	UPROPERTY(Category="SCPawnMovement", BlueprintReadOnly)
	TEnumAsByte<enum EMovementMode> MovementMode;

	/**
	* Current custom sub-mode if MovementMode is set to Custom.
	* This is automatically replicated through the Character owner and for client-server movement functions.
	* @see SetMovementMode()
	*/
	UPROPERTY(Category="Character Movement: MovementMode", BlueprintReadOnly)
	uint8 CustomMovementMode;

	/**
	* Whether or not the character should sweep for collision geometry while walking.
	* @see USceneComponent::MoveComponent.
	*/
	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite)
	uint32 bSweepWhileNavWalking:1;

private:

	// Tracks whether or not we need to update the bSweepWhileNavWalking flag do to an upgrade.
	uint32 bNeedsSweepWhileWalkingUpdate:1;

protected:

	/**
	* True during movement update.
	* Used internally so that attempts to change CharacterOwner and UpdatedComponent are deferred until after an update.
	* @see IsMovementInProgress()
	*/
	UPROPERTY()
	uint32 bMovementInProgress:1;

public:
	/** Ignores size of acceleration component, and forces max acceleration to drive character at full velocity. */
	UPROPERTY()
	uint32 bForceMaxAccel : 1;

	///** Information about the floor the Character is standing on (updated only during walking movement). */
	//UPROPERTY(Category = "Character Movement: Walking", VisibleInstanceOnly, BlueprintReadOnly)
	//FSCFindFloorResult CurrentFloor;

	/**
	* Default movement mode when not in water. Used at player startup or when teleported.
	* @see DefaultWaterMovementMode
	* @see bRunPhysicsWithNoController
	*/
	UPROPERTY(Category="Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<enum EMovementMode> DefaultLandMovementMode;

private:
	/**
	* Ground movement mode to switch to after falling and resuming ground movement.
	* Only allowed values are: MOVE_Walking, MOVE_NavWalking.
	* @see SetGroundMovementMode(), GetGroundMovementMode()
	*/
	UPROPERTY(Transient)
	TEnumAsByte<enum EMovementMode> GroundMovementMode;

public:
//	/**
//	* If true, walking movement always maintains horizontal velocity when moving up ramps, which causes movement up ramps to be faster parallel to the ramp surface.
//	* If false, then walking movement maintains velocity magnitude parallel to the ramp surface.
//	*/
//	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite)
//	uint32 bMaintainHorizontalGroundVelocity:1;

	/**
	* Change movement mode.
	*
	* @param NewMovementMode	The new movement mode
	* @param NewCustomMode		The new custom sub-mode, only applicable if NewMovementMode is Custom.
	*/
	UFUNCTION(BlueprintCallable, Category="Pawn|Components|CharacterMovement")
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0);

	/**
	* Set movement mode to use when returning to walking movement (either MOVE_Walking or MOVE_NavWalking).
	* If movement mode is currently one of Walking or NavWalking, this will also change the current movement mode (via SetMovementMode())
	* if the new mode is not the current ground mode.
	* 
	* @param  NewGroundMovementMode New ground movement mode. Must be either MOVE_Walking or MOVE_NavWalking, other values are ignored.
	* @see GroundMovementMode
	*/
	void SetGroundMovementMode(EMovementMode NewGroundMovementMode);

	/**
	* Get current GroundMovementMode value.
	* @return current GroundMovementMode
	* @see GroundMovementMode, SetGroundMovementMode()
	*/
	EMovementMode GetGroundMovementMode() const { return GroundMovementMode; }

	/** Get Navigation data for the Character. Returns null if there is no associated nav data. */
	const class ANavigationData* GetNavData() const;

protected:

	/** Called after MovementMode has changed. Base implementation does special handling for starting certain modes, then notifies the CharacterOwner. */
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode);

public:
//
//	virtual bool IsFalling() const override;
//	virtual float GetMaxSpeed() const override;
//	virtual void StopActiveMovement() override;
	virtual bool IsMovingOnGround() const override;
//	virtual float GetGravityZ() const override;
//
//	/** @return true if the character is in the 'Walking' movement mode. */
//	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement")
//	bool IsWalking() const;
//
//	/**
//	* @return true if currently performing a movement update.
//	* @see bMovementInProgress
//	*/
//	bool IsMovementInProgress() const { return bMovementInProgress; }
//
//	/** Make movement impossible (sets movement mode to MOVE_None). */
//	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement")
//	virtual void DisableMovement();
//
//	/** Adjust distance from floor, trying to maintain a slight offset from the floor when walking (based on CurrentFloor). */
//	virtual void AdjustFloorHeight();
//
//	/** Return PrimitiveComponent we are based on (standing and walking on). */
//	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement")
//	UPrimitiveComponent* GetMovementBase() const;
//
//	/** @return true if we can step up on the actor in the given FHitResult. */
//	virtual bool CanStepUp(const FHitResult& Hit) const;
//
//	/** 
//	* Move up steps or slope. Does nothing and returns false if CanStepUp(Hit) returns false.
//	*
//	* @param GravDir			Gravity vector direction (assumed normalized or zero)
//	* @param Delta				Requested move
//	* @param Hit				[In] The hit before the step up.
//	* @param OutStepDownResult	[Out] If non-null, a floor check will be performed if possible as part of the final step down, and it will be updated to reflect this result.
//	* @return true if the step up was successful.
//	*/
//	virtual bool StepUp(const FVector& GravDir, const FVector& Delta, const FHitResult &Hit, struct UCharacterMovementComponent::FStepDownResult* OutStepDownResult = NULL);
//
//	/** Check if pawn is falling */
//	virtual bool CheckFall(const FFindFloorResult& OldFloor, const FHitResult& Hit, const FVector& Delta, const FVector& OldLocation, float remainingTime, float timeTick, int32 Iterations, bool bMustJump);
//
//	/** Set movement mode to the default based on the current physics volume. */
//	virtual void SetDefaultMovementMode();
//
//	/** Return true if the hit result should be considered a walkable surface for the character. */
//	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement")
//	virtual bool IsWalkable(const FHitResult& Hit) const;
//
//	/** Get the max angle in degrees of a walkable surface for the character. */
//	FORCEINLINE float GetWalkableFloorAngle() const { return WalkableFloorAngle; }
//
//	/** Get the Z component of the normal of the steepest walkable surface for the character. Any lower than this and it is not walkable. */
//	FORCEINLINE float GetWalkableFloorZ() const { return WalkableFloorZ; }
//
//	/**
//	* Compute a vector of movement, given a delta and a hit result of the surface we are on.
//	*
//	* @param Delta:				Attempted movement direction
//	* @param RampHit:				Hit result of sweep that found the ramp below the capsule
//	* @param bHitFromLineTrace:	Whether the floor trace came from a line trace
//	*
//	* @return If on a walkable surface, this returns a vector that moves parallel to the surface. The magnitude may be scaled if bMaintainHorizontalGroundVelocity is true.
//	* If a ramp vector can't be computed, this will just return Delta.
//	*/
//	virtual FVector ComputeGroundMovementDelta(const FVector& Delta, const FHitResult& RampHit, const bool bHitFromLineTrace) const;
//
//	/**
//	* Move along the floor, using CurrentFloor and ComputeGroundMovementDelta() to get a movement direction.
//	* If a second walkable surface is hit, it will also be moved along using the same approach.
//	*
//	* @param InVelocity:			Velocity of movement
//	* @param DeltaSeconds:			Time over which movement occurs
//	* @param OutStepDownResult:	[Out] If non-null, and a floor check is performed, this will be updated to reflect that result.
//	*/
//	virtual void MoveAlongFloor(const FVector& InVelocity, float DeltaSeconds, FStepDownResult* OutStepDownResult = NULL);
//
//
//



public:
	/** Maximum velocity magnitude allowed for the controlled Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SCPawnMovement)
	FVector ImpactNormal;

	/** Maximum velocity magnitude allowed for the controlled Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SCPawnMovement)
	AActor* HitActor;

	/** Maximum velocity magnitude allowed for the controlled Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SCPawnMovement)
	float MaxSpeed;

	/** Maximum velocity magnitude allowed for the controlled Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SCPawnMovement)
	float Gravity;

	/** Acceleration applied by input (rate of change of velocity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SCPawnMovement)
	float Acceleration;

	/** Deceleration applied when there is no input (rate of change of velocity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SCPawnMovement)
	float Deceleration;

	/**
	* Setting affecting extra force applied when changing direction, making turns have less drift and become more responsive.
	* Velocity magnitude is not allowed to increase, that only happens due to normal acceleration. It may decrease with large direction changes.
	* Larger values apply extra force to reach the target direction more quickly, while a zero value disables any extra turn force.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FloatingPawnMovement, meta=(ClampMin="0", UIMin="0"))
	float TurningBoost;

	UPROPERTY(Transient)
	uint32 bPositionCorrected : 1;

	UFUNCTION()
	bool HasValidData() const;

	void ApplyControlInputToVelocity(float DeltaTime);

	/** Prevent Pawn from leaving the world bounds (if that restriction is enabled in WorldSettings) */
	virtual bool LimitWorldBounds();
};
