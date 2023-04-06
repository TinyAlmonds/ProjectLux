// Copyright TinyAlmonds (Alex Noerdemann)

#pragma once

#include "CoreMinimal.h"

#include "ProjectLuxMovementSpaceState.generated.h"

/** Enum indicating the space in which the player is able to move. */
UENUM(BlueprintType)
enum class EMovementSpaceState : uint8
{
	MovementIn2D,
	MovementIn3D,
	MovementOnSpline
};
