// Copyright TinyAlmonds (Alex Noerdemann)
#include "Core/PLPlayerController.h"

#include "Camera/CameraComponent.h"
#include "Core/PLCharacter.h"
#include "Core/UI/PLHUD.h"

APLPlayerController::APLPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    bAutoManageActiveCameraTarget = true;
}

UCameraComponent *APLPlayerController::GetViewTargetCameraComponent() const
{
    if (const AActor * ViewTarget{GetViewTarget()}; ViewTarget)
    {
        return ViewTarget->FindComponentByClass<UCameraComponent>();
    }

    return nullptr;
}

void APLPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Bind to input action/axis mappings which redirects to the controlled pawn
    InputComponent->BindAction("Jump", IE_Pressed, this, &APLPlayerController::JumpPress);
    InputComponent->BindAction("Jump", IE_Released, this, &APLPlayerController::JumpRelease);
    InputComponent->BindAxis("MoveUp", this, &APLPlayerController::MoveUp);
    InputComponent->BindAxis("MoveRight", this, &APLPlayerController::MoveRight);
    InputComponent->BindAction("Sprint", IE_Pressed, this, &APLPlayerController::SprintPress);
    InputComponent->BindAction("Sprint", IE_Released, this, &APLPlayerController::SprintRelease);
    InputComponent->BindAction("Dash", IE_Pressed, this, &APLPlayerController::DashPress);
    InputComponent->BindAction("QuickStep", IE_Pressed, this, &APLPlayerController::QuickStepPress);
    InputComponent->BindAction("Glide", IE_Pressed, this, &APLPlayerController::GlidePress);
    InputComponent->BindAction("Attack", IE_Pressed, this, &APLPlayerController::AttackPress);

    // Bind to input which redirects to the related HUD class
    InputComponent->BindAction("ShowPauseMenu", IE_Pressed, this, &APLPlayerController::ShowPauseMenuPress);
}

void APLPlayerController::DisableInput(class APlayerController *PlayerController)
{
    Super::DisableInput(PlayerController);

    // Reset/Release all potential key presses
    MoveUp(0.0f);
    MoveRight(0.0f);
    APLPlayerController::JumpRelease();
    APLPlayerController::SprintRelease();

    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->TryCancelGlideAbility();
        }
    }
}

void APLPlayerController::JumpPress()
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->JumpPress();
        }
    }
}

void APLPlayerController::JumpRelease()
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->JumpRelease();
        }
    }
}

void APLPlayerController::MoveRight(float AxisValue)
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->MoveRight(AxisValue);
        }
    }
}

void APLPlayerController::MoveUp(float AxisValue)
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->MoveUp(AxisValue);
        }
    }
}

void APLPlayerController::SprintPress()
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->SprintPress();
        }
    }
}

void APLPlayerController::SprintRelease()
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->SprintRelease();
        }
    }
}

void APLPlayerController::DashPress()
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->DashPress();
        }
    }
}

void APLPlayerController::QuickStepPress()
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->QuickStepPress();
        }
    }
}

void APLPlayerController::GlidePress()
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->GlidePress();
        }
    }
}

void APLPlayerController::AttackPress()
{
    APawn *PossessedPawn = GetPawn();
    if (PossessedPawn)
    {
        APLCharacter *LuxCharacter = Cast<APLCharacter>(PossessedPawn);
        if (LuxCharacter)
        {
            LuxCharacter->AttackPress();
        }
    }
}

void APLPlayerController::ShowPauseMenuPress()
{
    AHUD *BaseHUD{GetHUD()};
    if (BaseHUD)
    {
        APLHUD *LuxHUD = Cast<APLHUD>(BaseHUD);
        if (LuxHUD)
        {
            LuxHUD->ShowPauseMenuCalled();
        }
    }
}
