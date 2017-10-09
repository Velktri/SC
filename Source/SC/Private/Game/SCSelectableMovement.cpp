// Fill out your copyright notice in the Description page of Project Settings.

#include "SCSelectableMovement.h"
#include "SCSelectable.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/World.h"
#include "AI/Navigation/NavigationSystem.h"
#include "AI/Navigation/RecastNavMesh.h"

USCSelectableMovement::USCSelectableMovement()
{
	MaxSpeed = 1200.f;
	Acceleration = 4000.f;
	Deceleration = 8000.f;
	TurningBoost = 8.0f;
	Gravity = 100000.0f;
	GravityScale = 1.f;
	MaxStepHeight = 45.0f;

	DefaultLandMovementMode = MOVE_Walking;


	bPositionCorrected = false;
}

void USCSelectableMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	const FVector InputVector = ConsumeInputVector();
	if (!HasValidData() || ShouldSkipUpdate(DeltaTime)) { return; }

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HasValidData() || ShouldSkipUpdate(DeltaTime)) { return; }


	const AController* Controller = PawnOwner->GetController();
	if (Controller && Controller->IsLocalController())
	{
		// apply input for local players but also for AI that's not following a navigation path at the moment
		if (Controller->IsLocalPlayerController() == true || Controller->IsFollowingAPath() == false || bUseAccelerationForPaths)
		{
			ApplyControlInputToVelocity(DeltaTime);
		}
		// if it's not player controller, but we do have a controller, then it's AI
		// (that's not following a path) and we need to limit the speed
		else if (IsExceedingMaxSpeed(MaxSpeed) == true)
		{
			Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
		}

		/* Apply simple Gravity */
		//if (IsMovingOnGround())
		//{
			FHitResult Floor;
			const FVector Down = FVector(0.f, 0.f, -20000.f);
			FVector LineTraceStart = PawnOwner->GetActorLocation();
			LineTraceStart.Z = 10000;

			if (GetWorld()->LineTraceSingleByChannel(Floor, LineTraceStart, LineTraceStart + Down, COLLISION_GROUND))
			{
				FVector GravityDir = Floor.ImpactNormal;//FVector(0.0f, 0.0f, -1.0f);
				ImpactNormal = GravityDir;
				HitActor = Floor.Actor.Get();
				Velocity +=  -1.0f * GravityDir * Gravity * DeltaTime;
			}
		//}

		LimitWorldBounds();
		bPositionCorrected = false;

		// Move actor
		FVector Delta = Velocity * DeltaTime;

		if (!Delta.IsNearlyZero(1e-6f))
		{
			const FVector OldLocation = UpdatedComponent->GetComponentLocation();
			const FQuat Rotation = UpdatedComponent->GetComponentQuat();

			FHitResult Hit(1.f);
			SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

			if (Hit.IsValidBlockingHit())
			{
				HandleImpact(Hit, DeltaTime, Delta);
				// Try to slide the remaining distance along the surface.
				SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
			}

			// Update velocity
			// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
			if (!bPositionCorrected)
			{
				const FVector NewLocation = UpdatedComponent->GetComponentLocation();
				Velocity = ((NewLocation - OldLocation) / DeltaTime);
			}
		}

		// Finalize
		UpdateComponentVelocity();
	}
}

bool USCSelectableMovement::HasValidData() const
{
	return UpdatedComponent && PawnOwner;
}

void USCSelectableMovement::ApplyControlInputToVelocity(float DeltaTime)
{
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

	const float AnalogInputModifier = (ControlAcceleration.SizeSquared() > 0.f ? ControlAcceleration.Size() : 0.f);
	const float MaxPawnSpeed = GetMaxSpeed() * AnalogInputModifier;
	const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(MaxPawnSpeed);

	if (AnalogInputModifier > 0.f && !bExceedingMaxSpeed)
	{
		// Apply change in velocity direction
		if (Velocity.SizeSquared() > 0.f)
		{
			// Change direction faster than only using acceleration, but never increase velocity magnitude.
			const float TimeScale = FMath::Clamp(DeltaTime * TurningBoost, 0.f, 1.f);
			Velocity = Velocity + (ControlAcceleration * Velocity.Size() - Velocity) * TimeScale;
		}
	} else
	{
		// Dampen velocity magnitude based on deceleration.
		if (Velocity.SizeSquared() > 0.f)
		{
			const FVector OldVelocity = Velocity;
			const float VelSize = FMath::Max(Velocity.Size() - FMath::Abs(Deceleration) * DeltaTime, 0.f);
			Velocity = Velocity.GetSafeNormal() * VelSize;

			// Don't allow braking to lower us below max speed if we started above it.
			if (bExceedingMaxSpeed && Velocity.SizeSquared() < FMath::Square(MaxPawnSpeed))
			{
				Velocity = OldVelocity.GetSafeNormal() * MaxPawnSpeed;
			}
		}
	}

	// Apply acceleration and clamp velocity magnitude.
	const float NewMaxSpeed = (IsExceedingMaxSpeed(MaxPawnSpeed)) ? Velocity.Size() : MaxPawnSpeed;
	Velocity += ControlAcceleration * FMath::Abs(Acceleration) * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(NewMaxSpeed);

	ConsumeInputVector();
}

bool USCSelectableMovement::LimitWorldBounds()
{
	AWorldSettings* WorldSettings = PawnOwner ? PawnOwner->GetWorldSettings() : NULL;
	if (!WorldSettings || !WorldSettings->bEnableWorldBoundsChecks || !UpdatedComponent)
	{
		return false;
	}

	const FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	if (CurrentLocation.Z < WorldSettings->KillZ)
	{
		Velocity.Z = FMath::Min(GetMaxSpeed(), WorldSettings->KillZ - CurrentLocation.Z + 2.0f);
		return true;
	}

	return false;
}

void USCSelectableMovement::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode /*= 0*/)
{
	if (NewMovementMode != MOVE_Custom)
	{
		NewCustomMode = 0;
	}

	// If trying to use NavWalking but there is no navmesh, use walking instead.
	if (NewMovementMode == MOVE_NavWalking)
	{
		if (GetNavData() == nullptr)
		{
			NewMovementMode = MOVE_Walking;
		}
	}

	// Do nothing if nothing is changing.
	if (MovementMode == NewMovementMode)
	{
		// Allow changes in custom sub-mode.
		if ((NewMovementMode != MOVE_Custom) || (NewCustomMode == CustomMovementMode))
		{
			return;
		}
	}

	const EMovementMode PrevMovementMode = MovementMode;
	const uint8 PrevCustomMode = CustomMovementMode;

	MovementMode = NewMovementMode;
	CustomMovementMode = NewCustomMode;

	// We allow setting movement mode before we have a component to update, in case this happens at startup.
	if (!HasValidData())
	{
		return;
	}

	// Handle change in movement mode
	OnMovementModeChanged(PrevMovementMode, PrevCustomMode);

	// @todo UE4 do we need to disable ragdoll physics here? Should this function do nothing if in ragdoll?
}

void USCSelectableMovement::SetGroundMovementMode(EMovementMode NewGroundMovementMode)
{
	// Enforce restriction that it's either Walking or NavWalking.
	if (NewGroundMovementMode != MOVE_Walking && NewGroundMovementMode != MOVE_NavWalking)
	{
		return;
	}

	// Set new value
	GroundMovementMode = NewGroundMovementMode;

	// Possibly change movement modes if already on ground and choosing the other ground mode.
	const bool bOnGround = (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking);
	if (bOnGround && MovementMode != NewGroundMovementMode)
	{
		SetMovementMode(NewGroundMovementMode);
	}
}

const ANavigationData* USCSelectableMovement::GetNavData() const
{
	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
	if (!NavSys || !HasValidData()) { return nullptr; }

	const ANavigationData* NavData = nullptr;
	INavAgentInterface* MyNavAgent = CastChecked<INavAgentInterface>(PawnOwner);
	if (MyNavAgent)
	{
		const FNavAgentProperties& AgentProps = MyNavAgent->GetNavAgentPropertiesRef();
		NavData = NavSys->GetNavDataForProps(AgentProps);
	}
	if (NavData == nullptr)
	{
		NavData = NavSys->GetMainNavData();
	}

	// Only RecastNavMesh supported
	const ARecastNavMesh* NavMeshData = Cast<const ARecastNavMesh>(NavData);
	if (NavMeshData == nullptr)
	{
		return nullptr;
	}

	return NavData;
}

void USCSelectableMovement::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	//if (!HasValidData()) { return; }

	//// Update collision settings if needed
	//if (MovementMode == MOVE_NavWalking)
	//{
	//	SetNavWalkingPhysics(true);
	//	GroundMovementMode = MovementMode;
	//	// Walking uses only XY velocity
	//	Velocity.Z = 0.f;
	//} else if (PreviousMovementMode == MOVE_NavWalking)
	//{
	//	if (MovementMode == DefaultLandMovementMode || IsWalking())
	//	{
	//		const bool bSucceeded = TryToLeaveNavWalking();
	//		if (!bSucceeded)
	//		{
	//			return;
	//		}
	//	} else
	//	{
	//		SetNavWalkingPhysics(false);
	//	}
	//}

	//// React to changes in the movement mode.
	//if (MovementMode == MOVE_Walking)
	//{
	//	// Walking uses only XY velocity, and must be on a walkable floor, with a Base.
	//	Velocity.Z = 0.f;
	//	bCrouchMaintainsBaseLocation = true;
	//	GroundMovementMode = MovementMode;

	//	// make sure we update our new floor/base on initial entry of the walking physics
	//	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, false);
	//	AdjustFloorHeight();
	//	SetBaseFromFloor(CurrentFloor);
	//} else
	//{
	//	CurrentFloor.Clear();
	//	bCrouchMaintainsBaseLocation = false;

	//	if (MovementMode == MOVE_Falling)
	//	{
	//		Velocity += GetImpartedMovementBaseVelocity();
	//		CharacterOwner->Falling();
	//	}

	//	SetBase(NULL);

	//	if (MovementMode == MOVE_None)
	//	{
	//		// Kill velocity and clear queued up events
	//		StopMovementKeepPathing();
	//		CharacterOwner->ClearJumpInput();
	//		ClearAccumulatedForces();
	//	}
	//}

	//if (MovementMode == MOVE_Falling && PreviousMovementMode != MOVE_Falling && PathFollowingComp.IsValid())
	//{
	//	PathFollowingComp->OnStartedFalling();
	//}

	//CharacterOwner->OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	//ensure(GroundMovementMode == MOVE_Walking || GroundMovementMode == MOVE_NavWalking);
}

bool USCSelectableMovement::IsMovingOnGround() const
{
	return ((MovementMode == MOVE_Walking) || (MovementMode == MOVE_NavWalking)) && UpdatedComponent;
}