// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectLuxCharacter.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/Optional.h"



AProjectLuxCharacter::AProjectLuxCharacter() : 
	VelocityZWallSlide{-180.0f},
	bWallSlidingFlag{false}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 
	JumpMaxHoldTime = 0.35;

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->JumpZVelocity = 600.0f;
	}
}


void AProjectLuxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWallSlidingFlag();
}

void AProjectLuxCharacter::JumpPress()
{
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
}

bool AProjectLuxCharacter::GetWallSlidingFlag() const
{
	return bWallSlidingFlag;
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