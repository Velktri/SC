// Fill out your copyright notice in the Description page of Project Settings.

#include "SCPlayerView.h"
#include "Camera/CameraComponent.h"
#include "Engine.h"
#include "SCPlayerController.h"
#include "CameraBoundsVolume.h"
#include "PlayerMovement.h"

ASCPlayerView::ASCPlayerView()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraScrollBoundary = 10.0f;
	MousePosition = FVector2D(0, 0);
	FieldOfView = 40.0f;
	CameraRotation = FRotator(-45, 0, 0);
	CameraDistance = 5000.0f;
	CameraSpeedModifier = 50.0f;
	bCanMoveCamera = true;

	TransformX = FVector(0, 1, 0);
	TransformY = FVector(1, 0, 0);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(RootComponent);

#if defined(UE_BUILD_DEBUG) || defined(UE_EDITOR)
	DebugLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("DebugLight"));
	DebugLight->SetupAttachment(RootComponent);

	DebugLight->Intensity = 25000;
	DebugLight->bUseInverseSquaredFalloff = false;
	DebugLight->LightColor = FColor::Red;
	DebugLight->AddRelativeLocation(FVector(0, 0, 100));
	DebugLight->AttenuationRadius = 500;
#endif

	PlayerMovement = CreateDefaultSubobject<UPlayerMovement>(TEXT("PlayerMovementComponent"));
	PlayerMovement->UpdatedComponent = RootComponent;
}

void ASCPlayerView::BeginPlay()
{
	Super::BeginPlay();
	SetupCamera();
}

void ASCPlayerView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector2D viewportSize;
	UGameViewportClient* gameViewport = GEngine->GameViewport;

	//Make sure viewport exists
	check(gameViewport);
	gameViewport->GetViewportSize(viewportSize);

	//Make sure the viewport has focus(contains the mouse)
	if (gameViewport->IsFocused(gameViewport->Viewport) && gameViewport->GetMousePosition(MousePosition)) 
	{
		FVector Direction = FVector::ZeroVector;
		//Check if the mouse is at the left or right edge of the screen and move accordingly
		if (bCanScroll(MousePosition.X)) 
		{
			Direction += (TransformX * -1.0f);
		} 
		else if (bCanScroll(viewportSize.X - MousePosition.X)) 
		{
			Direction += (TransformX);
		}

		//Check if the mouse is at the top or bottom edge of the screen and move accordingly
		if (bCanScroll(MousePosition.Y)) 
		{
			Direction += TransformY;
		} 
		else if (bCanScroll(viewportSize.Y - MousePosition.Y)) 
		{
			Direction += (TransformY * -1.0f);
		}

		if (Direction != FVector::ZeroVector)
		{
			CalculateCameraDirection(Direction, DeltaTime);
		}
	}
}

void ASCPlayerView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveHorizontal", this, &ASCPlayerView::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &ASCPlayerView::MoveVertical);
}

UPawnMovementComponent* ASCPlayerView::GetMovementComponent() const {
	return PlayerMovement;
}

void ASCPlayerView::MoveVertical(float Speed) {
	CalculateCameraDirection(TransformY, Speed);
}

void ASCPlayerView::MoveHorizontal(float Speed) {
	CalculateCameraDirection(TransformX, Speed);
}

void ASCPlayerView::SetupCamera() 
{
	PlayerCamera->SetRelativeRotation(CameraRotation);
	PlayerCamera->SetFieldOfView(FieldOfView);

	FVector forwardVector = PlayerCamera->GetForwardVector() * -1.0f * CameraDistance;
	PlayerCamera->SetRelativeLocation(forwardVector);
	FVector loc = FVector(GetActorLocation());
	loc.Z = 0;
	SetActorLocation(loc);

	PlayerCamera->PostProcessSettings.bOverride_AutoExposureMinBrightness = 1;
	PlayerCamera->PostProcessSettings.AutoExposureMinBrightness = 1;

	PlayerCamera->PostProcessSettings.bOverride_AutoExposureMaxBrightness = 1;
	PlayerCamera->PostProcessSettings.AutoExposureMaxBrightness = 1;

	ASCPlayerController* PC = GetSCPlayerContorller();
	if (PC && PC->GetGameBounds())
	{
		TransformX = PC->GetGameBounds()->AxisX;
		TransformY = PC->GetGameBounds()->AxisY;
	}
}

void ASCPlayerView::CalculateCameraDirection(FVector Direction, float Distance) 
{
	Distance *= (10000.0f * CameraSpeedModifier);
	ASCPlayerController* PC = GetSCPlayerContorller();
	if (PC && PC->GetGameBounds()) 
	{
		FBox GameBounds = PC->GetGameBounds()->WorldBounds;
		bool bCurrentPointValid = PC->GetGameBounds()->EncompassesPoint(GetActorLocation());

		// check if outside bounds
		if (!bCurrentPointValid) 
		{
			int32 offset = 1.0f;
			FVector newLocation = GetActorLocation();
			FVector GameExtents = GameBounds.GetExtent();
			if (FMath::Abs(GetActorLocation().X) > GameExtents.X)
			{
				if (GetActorLocation().X > 0)
				{
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Green, FString::Printf(TEXT("Location Error: %f, %f"), GetActorLocation().X, GetActorLocation().Y));
					}

					newLocation.X = GameExtents.X - offset;
					SetActorLocation(newLocation);
				}
				else 
				{
					newLocation.X = (GameExtents.X * -1.0f) + offset;
					SetActorLocation(newLocation);
				}
			}

			if (FMath::Abs(GetActorLocation().Y) > GameExtents.Y) {
				if (GetActorLocation().Y > 0) {
					newLocation.Y = GameExtents.Y - offset;
					SetActorLocation(newLocation);
				} else {
					newLocation.Y = (GameExtents.Y * -1.0f) + offset;
					SetActorLocation(newLocation);
				}
			}
		}
		else if (Distance != 0.0f) 
		{
			AddMovementInput(Direction, Distance);
		}
	}
}

bool ASCPlayerView::bCanScroll(float InBound)
{
	return (InBound < CameraScrollBoundary && bCanMoveCamera) ? true : false;
}

ASCPlayerController* ASCPlayerView::GetSCPlayerContorller() 
{
	return Cast<ASCPlayerController>(GetController());
}

