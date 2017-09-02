// Fill out your copyright notice in the Description page of Project Settings.

#include "RTS_PlayerMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"


UPlayerMovementComponent::UPlayerMovementComponent()
{
	MaxSpeed = 10000.f;
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (!HasValidData() || ShouldSkipUpdate(DeltaTime)) { return; }

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!HasValidData() || ShouldSkipUpdate(DeltaTime)) { return; }

	const APlayerController* Controller = Cast<APlayerController>(PawnOwner->GetController());
	if (Controller && Controller->IsLocalController())
	{
		Velocity = ConsumeInputVector();
		if (IsExceedingMaxSpeed(MaxSpeed))
		{
			Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
		}

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
		}

		UpdateComponentVelocity();
	}
}

bool UPlayerMovementComponent::HasValidData()
{
	return UpdatedComponent && IsValid(PawnOwner);
}
