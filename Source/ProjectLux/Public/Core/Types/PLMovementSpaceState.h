// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "CoreMinimal.h"

#include "PLMovementSpaceState.generated.h"

/** Enum indicating the space in which the player is able to move. */
UENUM(BlueprintType)
enum class EPLMovementSpaceState : uint8
{
	MovementIn2D,
	MovementIn3D,
	MovementOnSpline
};
