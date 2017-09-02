// Fill out your copyright notice in the Description page of Project Settings.

#include "RTS_PlayerView.h"
#include "Camera/CameraComponent.h"
#include "RTS_PlayerMovementComponent.h"
#include "Components/InputComponent.h"
#include "Engine.h"

FName APlayerView::CollisionComponentName(TEXT("CollisionComponent0"));

APlayerView::APlayerView()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraScrollBoundary = 10.0f;
	FieldOfView = 40.0f;
	CameraRotationPitch = -45.0f;
	CameraYawRotation = 0.0f;

	CameraMouseSpeedModifier = 10.0f;
	CameraKeyboardSpeedModifier = 10.0f;

	CameraDistance = 5000.0f;
	CameraDistanceModifer = 20.0f;
	MinCameraDistance = 50.0f;
	MaxCameraDistance = 10000.0f;

	bCanMoveCamera = true;

	Transform_X = FVector::ZeroVector;
	Transform_Y = FVector::ZeroVector;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(APlayerView::CollisionComponentName);
	CollisionComponent->InitSphereRadius(35.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CollisionComponent->SetCollisionObjectType(ECC_Destructible);

	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	CollisionComponent->SetCanEverAffectNavigation(false);
	RootComponent = CollisionComponent;

	CameraBoom = CreateDefaultSubobject<USphereComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(CameraBoom);
	PlayerCamera->PostProcessSettings.bOverride_AutoExposureMinBrightness = 1;
	PlayerCamera->PostProcessSettings.AutoExposureMinBrightness = 1;
	PlayerCamera->PostProcessSettings.bOverride_AutoExposureMaxBrightness = 1;
	PlayerCamera->PostProcessSettings.AutoExposureMaxBrightness = 1;


	PlayerMovement = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("PlayerMovementComponent"));
	PlayerMovement->UpdatedComponent = RootComponent;
}

void APlayerView::BeginPlay()
{
	Super::BeginPlay();

	RotateCameraPitch(CameraRotationPitch);
	UpdateCamera();

	SetActorRotation(FRotator(0, CameraYawRotation, 0));
	if (Transform_X == FVector::ZeroVector) { Transform_X = RootComponent->GetForwardVector(); }
	if (Transform_Y == FVector::ZeroVector) { Transform_Y = RootComponent->GetRightVector(); }

	FVector loc = FVector(GetActorLocation());
	loc.Z = 0;
	SetActorLocation(loc);
}

void APlayerView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D viewportSize = FVector2D::ZeroVector;
	UGameViewportClient* gameViewport = GEngine->GameViewport;

	//Make sure viewport exists
	check(gameViewport);
	gameViewport->GetViewportSize(viewportSize);

	//Make sure the viewport has focus(contains the mouse)
	FVector2D MousePosition = FVector2D::ZeroVector;
	if (gameViewport->IsFocused(gameViewport->Viewport) && gameViewport->GetMousePosition(MousePosition))
	{
		FVector Direction = FVector::ZeroVector;
		//Check if the mouse is at the left or right edge of the screen and move accordingly
		if (bCanScroll(MousePosition.X))
		{
			Direction += (Transform_Y * -1.0f);
		} 
		else if (bCanScroll(viewportSize.X - MousePosition.X))
		{
			Direction += (Transform_Y);
		}

		//Check if the mouse is at the top or bottom edge of the screen and move accordingly
		if (bCanScroll(MousePosition.Y))
		{
			Direction += Transform_X;
		} 
		else if (bCanScroll(viewportSize.Y - MousePosition.Y)) 
		{
			Direction += (Transform_X * -1.0f);
		}

		if (Direction != FVector::ZeroVector) 
		{
			CalculateCameraDirection(Direction * CameraMouseSpeedModifier, 1.0f);
		}
	}
}

void APlayerView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveHorizontal", this, &APlayerView::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &APlayerView::MoveVertical);
	PlayerInputComponent->BindAxis("RotateCamera", this, &APlayerView::RotateCameraYaw);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &APlayerView::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &APlayerView::ZoomOut);
	PlayerInputComponent->BindAction("ResetCameraRotation", IE_Pressed, this, &APlayerView::ResetCameraRotation);
}

UPawnMovementComponent* APlayerView::GetMovementComponent() const 
{
	return PlayerMovement;
}

void APlayerView::MoveVertical(float Speed)
{
	CalculateCameraDirection(Transform_X, Speed * CameraKeyboardSpeedModifier);
}

void APlayerView::MoveHorizontal(float Speed)
{
	CalculateCameraDirection(Transform_Y, Speed * CameraKeyboardSpeedModifier);
}

void APlayerView::ZoomIn()
{
	if (CameraDistance > (MinCameraDistance + (10 * CameraDistanceModifer))) {
		CameraDistance -= 10 * CameraDistanceModifer;
	}
	else
	{
		CameraDistance = MinCameraDistance;
	}
	UpdateCamera();
}

void APlayerView::ZoomOut()
{
	if (CameraDistance < (MaxCameraDistance - (10 * CameraDistanceModifer)))
	{
		CameraDistance += 10 * CameraDistanceModifer;
	}
	else
	{
		CameraDistance = MaxCameraDistance;
	}
	UpdateCamera();
}

void APlayerView::UpdateCamera()
{
	PlayerCamera->SetFieldOfView(FieldOfView);
	PlayerCamera->SetRelativeLocation(FVector(-1.0f * CameraDistance, 0, 0));
}

void APlayerView::RotateCameraYaw(float InAmount)
{
	AddActorWorldRotation(FRotator(0, InAmount, 0));
	Transform_X = RootComponent->GetForwardVector();
	Transform_Y = RootComponent->GetRightVector();
}

void APlayerView::RotateCameraPitch(float InAmount) {
	CameraBoom->AddLocalRotation(FRotator(InAmount, 0, 0));
}

void APlayerView::ResetCameraRotation()
{
	SetActorRotation(FRotator(0, CameraYawRotation, 0));
	Transform_X = RootComponent->GetForwardVector();
	Transform_Y = RootComponent->GetRightVector();
}

void APlayerView::CalculateCameraDirection(FVector Direction, float Distance)
{
	Distance *= 100.0f;
	if (Distance != 0.0f)
	{
		AddMovementInput(Direction, Distance);
	}
}

bool APlayerView::bCanScroll(float InBound) 
{
	return (InBound < CameraScrollBoundary && bCanMoveCamera) ? true : false;
}