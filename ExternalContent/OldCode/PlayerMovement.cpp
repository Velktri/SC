// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "SCPlayerController.h"

UPlayerMovement::UPlayerMovement()
{
	MaxSpeed = 10000.f;
}

bool UPlayerMovement::HasValidData() {
	return UpdatedComponent && IsValid(PawnOwner);
}

void UPlayerMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) 
{	
	if (!HasValidData() || ShouldSkipUpdate(DeltaTime))
	{
		return; 
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HasValidData() || ShouldSkipUpdate(DeltaTime)) 
	{ 
		return;
	}

	const ASCPlayerController* Controller = Cast<ASCPlayerController>(PawnOwner->GetController());
	if (Controller && Controller->IsLocalController()) 
	{
		Velocity = ConsumeInputVector();
		if (IsExceedingMaxSpeed(MaxSpeed)) 
		{
			Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
		}

		// Move actor
		FVector Delta = Velocity * DeltaTime;

		if (!Delta.IsNearlyZero(1e-6f)) 
		{
			const FVector OldLocation = UpdatedComponent->GetComponentLocation();
			const FQuat Rotation = UpdatedComponent->GetComponentQuat();

			FHitResult Hit(1.f);
			SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

			//if (Hit.IsValidBlockingHit()) 
			//{
			//	HandleImpact(Hit, DeltaTime, Delta);
			//	// Try to slide the remaining distance along the surface.
			//	SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
			//}

			// Update velocity
			// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
			//if (!bPositionCorrected) {
			//const FVector NewLocation = UpdatedComponent->GetComponentLocation();
			//Velocity = ((NewLocation - OldLocation) / DeltaTime);
			//}
		}

		// Finalize
		UpdateComponentVelocity();
	}
}