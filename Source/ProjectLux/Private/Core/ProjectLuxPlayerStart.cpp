// Copyright TinyAlmonds (Alex Noerdemann)
#include "Core/ProjectLuxPlayerStart.h"

EMovementSpaceState AProjectLuxPlayerStart::GetSpawnMovementSpaceState() const
{
    return MovementSpaceSpawn;
}

const USplineComponent *AProjectLuxPlayerStart::GetSpawnMovementSplineComponent()
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
