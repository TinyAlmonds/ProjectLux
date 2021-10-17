// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectLuxPlayerController.h"

AProjectLuxPlayerController::AProjectLuxPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AProjectLuxPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Bind to input action/axis mappings which redirects to the controlled pawn
    InputComponent->BindAction("Jump", IE_Pressed, this, &AProjectLuxPlayerController::JumpPressed);
    InputComponent->BindAction("Jump", IE_Released, this, &AProjectLuxPlayerController::JumpReleased);
    InputComponent->BindAxis("MoveUp", this, &AProjectLuxPlayerController::MoveUp);
    InputComponent->BindAxis("MoveRight", this, &AProjectLuxPlayerController::MoveRight);
}

void AProjectLuxPlayerController::JumpPressed()
{
}

void AProjectLuxPlayerController::JumpReleased()
{
}

void AProjectLuxPlayerController::MoveRight(float AxisValue)
{
}

void AProjectLuxPlayerController::MoveUp(float AxisValue)
{
}