#include "Core/ProjectLuxPlayerController.h"
#include "Core/ProjectLuxCharacter.h"


AProjectLuxPlayerController::AProjectLuxPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    bAutoManageActiveCameraTarget = false;
}

void AProjectLuxPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Bind to input action/axis mappings which redirects to the controlled pawn
    InputComponent->BindAction("Jump", IE_Pressed, this, &AProjectLuxPlayerController::JumpPress);
    InputComponent->BindAction("Jump", IE_Released, this, &AProjectLuxPlayerController::JumpRelease);
    InputComponent->BindAxis("MoveUp", this, &AProjectLuxPlayerController::MoveUp);
    InputComponent->BindAxis("MoveRight", this, &AProjectLuxPlayerController::MoveRight);
    InputComponent->BindAction("Sprint", IE_Pressed, this, &AProjectLuxPlayerController::SprintPress);
    InputComponent->BindAction("Sprint", IE_Released, this, &AProjectLuxPlayerController::SprintRelease);
    InputComponent->BindAction("Dash", IE_Pressed, this, &AProjectLuxPlayerController::DashPress);
    InputComponent->BindAction("QuickStep", IE_Pressed, this, &AProjectLuxPlayerController::QuickStepPress);
    InputComponent->BindAction("Glide", IE_Pressed, this, &AProjectLuxPlayerController::GlidePress);
    InputComponent->BindAction("Glide", IE_Released, this, &AProjectLuxPlayerController::GlideRelease);
    InputComponent->BindAction("Attack", IE_Pressed, this, &AProjectLuxPlayerController::AttackPress);
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

void AProjectLuxPlayerController::SprintPress()
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->SprintPress();
        }
    }
}

void AProjectLuxPlayerController::SprintRelease()
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->SprintRelease();
        }
    }
}

void AProjectLuxPlayerController::DashPress()
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->DashPress();
        }
    }
}

void AProjectLuxPlayerController::QuickStepPress()
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->QuickStepPress();
        }
    }
}

void AProjectLuxPlayerController::GlidePress()
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->GlidePress();
        }
    }
}

void AProjectLuxPlayerController::GlideRelease()
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->GlideRelease();
        }
    }
}

void AProjectLuxPlayerController::AttackPress()
{
    APawn* PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        AProjectLuxCharacter* LuxCharacter = Cast<AProjectLuxCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->AttackPress();
        }
    }
}
