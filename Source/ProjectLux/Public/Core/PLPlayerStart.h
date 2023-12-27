// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "Camera/CameraActor.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"

#include "Types/PLMovementSpaceState.h"
#include "PLPlayerStart.generated.h"

/**
 * Custom PlayerStart class for the project.
 */
UCLASS()
class PROJECTLUX_API APLPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	/**
	 * Returns the value of the movement space state for the spawning player.
	 * @return The value of the movement space state member.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement Space")
	EPLMovementSpaceState GetSpawnMovementSpaceState() const;

	/**
	 * Returns the USplineComponent in the world on which the spawned player can move, if in the EPLMovementSpaceState::MovementOnSpline state.
	 * @return The spline component member. Has to be checked for validness.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement Space")
	const USplineComponent *GetSpawnMovementSplineComponent();

	/**
	 * Returns the ACameraActor in the world used on spawn.
	 * @return The camera actor member. Has to be checked for validness.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement Space")
	const ACameraActor *GetSpawnCamera();

protected:
	/** Member indicating the space the spawned player is able to move in.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	EPLMovementSpaceState MovementSpaceSpawn{EPLMovementSpaceState::MovementIn2D};

	/** Reference to an AActor in the world which holds the USplineComponent on which the player can move, if in the EPLMovementSpaceState::MovementOnSpline state.*/
	UPROPERTY(EditAnywhere, Category = "Config", meta = (EditCondition = "MovementSpaceSpawn == EPLMovementSpaceState::MovementOnSpline", EditConditionHides))
	TWeakObjectPtr<AActor> MovementSplineActorSpawn{nullptr};

	/** Reference to the USplineComponent of the Actor in the world which holds the USplineComponent on which the player can move, if in the EPLMovementSpaceState::MovementOnSpline state.*/
	UPROPERTY()
	TWeakObjectPtr<USplineComponent> MovementSplineComponentFromWorld{nullptr};

	/** Reference to an ACameraActor in the world to use on spawn. If not set, we use the default camera of the player.*/
	UPROPERTY(EditAnywhere, Category = "Config")
	TWeakObjectPtr<ACameraActor> SpawnCamera{nullptr};
};
