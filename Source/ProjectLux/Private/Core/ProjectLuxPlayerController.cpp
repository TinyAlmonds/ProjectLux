// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectLuxPlayerController.h"
#include "Core/ProjectLuxCharacter.h"


AProjectLuxPlayerController::AProjectLuxPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AProjectLuxPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Bind to input action/axis mappings which redirects to the controlled pawn
    InputComponent->BindAction("Jump", IE_Pressed, this, &AProjectLuxPlayerController::JumpPress);
    InputComponent->BindAction("Jump", IE_Released, this, &AProjectLuxPlayerController::JumpRelease);
    InputComponent->BindAxis("MoveUp", this, &AProjectLuxPlayerController::MoveUp);
    InputComponent->BindAxis("MoveRight", this, &AProjectLuxPlayerController::MoveRight);
}

void AProjectLuxPlayerController::JumpPress()
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->JumpPress();
        }
    }
}

void AProjectLuxPlayerController::JumpRelease()
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->JumpRelease();
        }
    }
}

void AProjectLuxPlayerController::MoveRight(float AxisValue)
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->MoveRight(AxisValue);
        }
    }
}

void AProjectLuxPlayerController::MoveUp(float AxisValue)
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->MoveUp(AxisValue);
        }
    }
}