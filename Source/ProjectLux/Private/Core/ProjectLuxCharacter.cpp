// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectLuxCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"



AProjectLuxCharacter::AProjectLuxCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 
	JumpKeyHoldTime = 0.35;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->JumpZVelocity = 600.0f;
	}

}


void AProjectLuxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectLuxCharacter::JumpPress()
{
}

void AProjectLuxCharacter::JumpRelease()
{
}

void AProjectLuxCharacter::MoveRight(float AxisValue)
{
}

void AProjectLuxCharacter::MoveUp(float AxisValue)
{
}


void AProjectLuxCharacter::BeginPlay()
{
	Super::BeginPlay();
}
