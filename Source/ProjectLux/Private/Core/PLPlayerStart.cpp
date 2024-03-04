// Copyright TinyAlmonds (Alex Noerdemann)
#include "Core/PLPlayerStart.h"

EPLMovementSpaceState APLPlayerStart::GetSpawnMovementSpaceState() const
{
    return MovementSpaceSpawn;
}

const USplineComponent *APLPlayerStart::GetSpawnMovementSplineComponent()
{
    if (!MovementSplineComponentFromWorld.IsValid())
    {
        if (MovementSplineActorSpawn.IsValid())
        {
            USplineComponent *SplineComponent = MovementSplineActorSpawn.Get()->FindComponentByClass<USplineComponent>();
            if (IsValid(SplineComponent))
            {
                MovementSplineComponentFromWorld = SplineComponent;
            }
        }
    }

    return MovementSplineComponentFromWorld.Get();
}

const ACameraActor *APLPlayerStart::GetSpawnCamera()
{
    return SpawnCamera.Get();
}
