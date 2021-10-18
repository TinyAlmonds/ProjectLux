// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectLuxCharacter.h"
#include "Components/SplineComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/Optional.h"



AProjectLuxCharacter::AProjectLuxCharacter() : 
	VelocityZWallSlide{-180.0f},
	VelocityXYMultiplierWallJump{1.8f},
	VelocityZMultiplierWallJump{1.8f},
	AxisValueMoveUp{0.0f},
	AxisValueMoveRight{0.0f},
	bWallSlidingFlag{false},
	MovementSpace{EMovementSpaceState::MovementIn3D},
	PreviousMovementSpace{EMovementSpaceState::MovementIn3D},
	MovementSplineComponentFromWorld{nullptr}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 
	JumpMaxHoldTime = 0.35;

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->GravityScale = 5.5f;
		CharacterMovementComponent->MaxWalkSpeed = 600.0f;
		CharacterMovementComponent->JumpZVelocity = 1'000.0f;
	}
}


void AProjectLuxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWallSlidingFlag();

	// snap Character to closest Spline location, when in Spline Movement
	if ((MovementSpace == EMovementSpaceState::MovementOnSpline) && MovementSplineComponentFromWorld)
	{
		FVector CharacterWorldLocation = GetRootComponent()->GetComponentLocation();
		// we only want to find the closest location on the spline in the XY plane, since the Character can move freely in the Z direction
		FVector ClosestWorldLocationOnSpline = MovementSplineComponentFromWorld->FindLocationClosestToWorldLocation(FVector{CharacterWorldLocation.X, CharacterWorldLocation.Y, 0.0f}, ESplineCoordinateSpace::World);

		GetRootComponent()->SetWorldLocation(FVector{ClosestWorldLocationOnSpline.X, ClosestWorldLocationOnSpline.Y, CharacterWorldLocation.Z});
	}


}

void AProjectLuxCharacter::JumpPress()
{
	if (GetWallSlidingFlag() == true)
	{
		WallJump();
	}
	else
	{
		Jump();
	}
}

void AProjectLuxCharacter::JumpRelease()
{
	StopJumping();
}

void AProjectLuxCharacter::MoveRight(float AxisValue)
{
	AxisValueMoveRight = AxisValue;

	if (AxisValue != 0.0f)
	{
		FVector MovementDirection(0.0f, 0.0f, 0.0f);
		switch (MovementSpace)
		{
		case EMovementSpaceState::MovementIn2D:
		case EMovementSpaceState::MovementIn3D:
			MovementDirection.Y = AxisValue;
			AddMovementInput(MovementDirection);
			break;
		case EMovementSpaceState::MovementOnSpline:
			if (MovementSplineComponentFromWorld)
			{
				MovementDirection = AxisValue * MovementSplineComponentFromWorld->FindTangentClosestToWorldLocation(GetRootComponent()->GetComponentLocation(), ESplineCoordinateSpace::World);
				AddMovementInput(MovementDirection);
			}
			break;
		default:
			break;
		}
	}
}

void AProjectLuxCharacter::MoveUp(float AxisValue)
{
	AxisValueMoveUp = AxisValue;

	if (AxisValue != 0.0f)
	{
		FVector MovementDirection(0.0f, 0.0f, 0.0f);
		switch (MovementSpace)
		{
		case EMovementSpaceState::MovementIn2D:
			break;
		case EMovementSpaceState::MovementIn3D:
			MovementDirection.X = AxisValue;
			AddMovementInput(MovementDirection);
			break;
		case EMovementSpaceState::MovementOnSpline:
			break;
		default:
			break;
		}
	}
}

bool AProjectLuxCharacter::GetWallSlidingFlag() const
{
	return bWallSlidingFlag;
}

EMovementSpaceState AProjectLuxCharacter::GetMovementSpaceState() const
{
	return MovementSpace;
}

void AProjectLuxCharacter::SetMovementSpaceState(EMovementSpaceState State)
{
	PreviousMovementSpace = MovementSpace;
	MovementSpace = State;

	OnMovementSpaceStateChanged();
}

void AProjectLuxCharacter::SetMovementSpline(USplineComponent* MovementSplineComponent)
{
	MovementSplineComponentFromWorld = MovementSplineComponent;
}


void AProjectLuxCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectLuxCharacter::UpdateWallSlidingFlag()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();

	if (CharacterMovementComponent->IsFalling())
	{
		if (IsTouchingWallForWallSlide())
		{
			SetWallSlidingFlag(true);
		}
		else
		{
			SetWallSlidingFlag(false);
		}
	}
	else
	{
		SetWallSlidingFlag(false);
	}
}

void AProjectLuxCharacter::SetWallSlidingFlag(bool bFlagValue)
{
	bWallSlidingFlag = bFlagValue;

	OnWallSlidingFlagChanged();
}

void AProjectLuxCharacter::OnWallSlidingFlagChanged()
{
	if (GetWallSlidingFlag() == true)
	{
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			UMovementComponent* MovementComponent = Cast<UMovementComponent>(CharacterMovementComponent);
			if (MovementComponent)
			{
				TOptional<FHitResult> WallHit = IsTouchingWallForWallSlide();
				if (WallHit)
				{
					AController* PossessingController = GetController();
					if (PossessingController)
					{
						// rotate Character to face towards the negated normal of the wall
						FRotator RotationToFaceWall = (-(WallHit.GetValue().Normal)).Rotation();
						PossessingController->SetControlRotation(RotationToFaceWall);

						// let the Character slide down the wall on the specified velocity
						FVector const CurrentVelocity = CharacterMovementComponent->GetLastUpdateVelocity();
						MovementComponent->Velocity = FVector{ CurrentVelocity.X, CurrentVelocity.Y, VelocityZWallSlide };

					}
				}
			}
		}
	}
}

void AProjectLuxCharacter::OnMovementSpaceStateChanged()
{
	if (MovementSpace != PreviousMovementSpace)
	{
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			switch (MovementSpace)
			{
			case EMovementSpaceState::MovementIn2D:
				switch (PreviousMovementSpace)
				{
				case EMovementSpaceState::MovementIn2D:
					break;
				case EMovementSpaceState::MovementIn3D:
				case EMovementSpaceState::MovementOnSpline:
					CharacterMovementComponent->SetPlaneConstraintEnabled(true);
					CharacterMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);
					break;
				default:
					break;
				}
				break;
			case EMovementSpaceState::MovementIn3D:
			case EMovementSpaceState::MovementOnSpline:
				switch (PreviousMovementSpace)
				{
				case EMovementSpaceState::MovementIn2D:
					CharacterMovementComponent->SetPlaneConstraintEnabled(false);
					break;
				case EMovementSpaceState::MovementIn3D:
				case EMovementSpaceState::MovementOnSpline:
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}		
	}
}

void AProjectLuxCharacter::WallJump()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	AController* PossessingController = GetController();
	if (CharacterMovementComponent && PossessingController)
	{
		// Launch Character in opposite direction of its Forward Vector with the specified velocity
		FVector LaunchDirection = -GetActorForwardVector();
		FVector LaunchVelocity = LaunchDirection * CharacterMovementComponent->MaxWalkSpeed * VelocityXYMultiplierWallJump;
		LaunchVelocity.Z = CharacterMovementComponent->JumpZVelocity * VelocityZMultiplierWallJump;

		LaunchCharacter(LaunchVelocity, false, true);

		// Rotate Character to launch direction
		PossessingController->SetControlRotation(LaunchDirection.Rotation());
	}
}

TOptional<FHitResult> AProjectLuxCharacter::IsTouchingWallForWallSlide() const
{
	FHitResult OutWallHit;
	FVector LineTraceStart = GetActorLocation();
	FVector LineTraceEnd = LineTraceStart + (GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.1f));
	FName LineTraceProfileName = FName(TEXT("IgnoreOnlyPawn"));
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutWallHit, LineTraceStart, LineTraceEnd, ECC_Visibility, CollisionParams))
	{
		return TOptional<FHitResult>{OutWallHit};
	}

	return TOptional<FHitResult>{};
}