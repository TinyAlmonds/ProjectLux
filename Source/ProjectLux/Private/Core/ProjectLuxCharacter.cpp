// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectLuxCharacter.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/Optional.h"



AProjectLuxCharacter::AProjectLuxCharacter() : 
	VelocityZWallSlide{-180.0f},
	VelocityXYMultiplierWallJump{1.8f},
	VelocityZMultiplierWallJump{1.8f},
	AxisValueMoveUp{0.0f},
	bWallSlidingFlag{false},
	MovementSpace{EMovementSpaceState::MovementIn3D},
	PreviousMovementSpace{EMovementSpaceState::MovementIn3D}
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
}

void AProjectLuxCharacter::MoveUp(float AxisValue)
{
	AxisValueMoveUp = AxisValue;
	if (AxisValue != 0.0f)
	{
		FVector MovementDirection;
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