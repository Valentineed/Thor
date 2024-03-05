// Copyright Epic Games, Inc. All Rights Reserved.

#include "PandaProjectPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "PandaProjectCharacter.h"
#include "Kismet/GameplayStatics.h"

APandaProjectPlayerController::APandaProjectPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APandaProjectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void APandaProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Attack", IE_Pressed, this, &APandaProjectPlayerController::StartAttack);
	InputComponent->BindAction("Attack", IE_Released, this, &APandaProjectPlayerController::StopAttack);

	InputComponent->BindTouch(IE_Pressed, this, &APandaProjectPlayerController::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &APandaProjectPlayerController::TouchStopped);

	InputComponent->BindAction("Jump", IE_Pressed, this, &APandaProjectPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &APandaProjectPlayerController::StopJumping);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &APandaProjectPlayerController::OnResetVR);
}

void APandaProjectPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	APandaProjectCharacter* CharacterPlayer = Cast<APandaProjectCharacter>(InPawn);
	if (CharacterPlayer != nullptr)
	{
		InputComponent->BindAxis("MoveForward", CharacterPlayer, &APandaProjectCharacter::MoveForward);
		InputComponent->BindAxis("MoveRight", CharacterPlayer, &APandaProjectCharacter::MoveRight);

		InputComponent->BindAction("Ultimate", IE_Pressed, CharacterPlayer, &APandaProjectCharacter::Ultimate);

		InputComponent->BindAction("Capacity", IE_Pressed, CharacterPlayer, &APandaProjectCharacter::Capacity);
	}
}

void APandaProjectPlayerController::StartAttack()
{
	APawn* const MyPawn = GetPawn();
	if(MyPawn != nullptr)
	{
		Cast<APandaProjectCharacter>(MyPawn)->StartAttack();
	}
}

void APandaProjectPlayerController::StopAttack()
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn != nullptr)
	{
		Cast<APandaProjectCharacter>(MyPawn)->StopAttack();
	}
}

void APandaProjectPlayerController::Jump()
{
	if (APandaProjectCharacter* MyPawn = Cast<APandaProjectCharacter>(GetPawn()))
	{
		MyPawn->LoadJumpAttack();
	}
}

void APandaProjectPlayerController::StopJumping()
{
	if (APandaProjectCharacter* MyPawn = Cast<APandaProjectCharacter>(GetPawn()))
	{
		MyPawn->JumpAttack();
	}
}

void APandaProjectPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APandaProjectPlayerController::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (APandaProjectCharacter* MyPawn = Cast<APandaProjectCharacter>(GetPawn()))
	{
		MyPawn->Jump();
	}
}

void APandaProjectPlayerController::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (APandaProjectCharacter* MyPawn = Cast<APandaProjectCharacter>(GetPawn()))
	{
		MyPawn->StopJumping();
	}
}

void APandaProjectPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (APandaProjectCharacter* MyPawn = Cast<APandaProjectCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void APandaProjectPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void APandaProjectPlayerController::MoveForward(float Value)
{
	if (APandaProjectCharacter* MyPawn = Cast<APandaProjectCharacter>(GetPawn()))
	{
		MyPawn->MoveForward(Value);
	}
}

void APandaProjectPlayerController::MoveRight(float Value)
{
	if (APandaProjectCharacter* MyPawn = Cast<APandaProjectCharacter>(GetPawn()))
	{
		MyPawn->MoveRight(Value);
	}
}

void APandaProjectPlayerController::TurnAtRate(float Rate)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		// calculate delta for this frame from the rate information
		MyPawn->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void APandaProjectPlayerController::LookUpAtRate(float Rate)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		// calculate delta for this frame from the rate information
		MyPawn->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void APandaProjectPlayerController::Attack(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);
	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
}

void APandaProjectPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APandaProjectPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void APandaProjectPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
