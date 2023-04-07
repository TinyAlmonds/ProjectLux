// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"

#include "Types/ProjectLuxMovementSpaceState.h"
#include "ProjectLuxPlayerStart.generated.h"

/**
 * Custom PlayerStart class for the project.
 */
UCLASS()
class PROJECTLUX_API AProjectLuxPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	/**
	 * Returns the value of the movement space state for the spawning player.
	 * @return The value of the movement space state member.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement Space")
	EMovementSpaceState GetSpawnMovementSpaceState() const;

	/**
	 * Returns the USplineComponent in the world on which the spawned player can move, if in the EMovementSpaceState::MovementOnSpline state.
	 * @return The spline component member. Has to be checked for validness.
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement Space")
	const USplineComponent *GetSpawnMovementSplineComponent();

protected:
	/** Member indicating the space the spawned player is able to move in.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	EMovementSpaceState MovementSpaceSpawn{EMovementSpaceState::MovementIn2D};

	/** Reference to an AActor in the world which holds the USplineComponent on which the player can move, if in the EMovementSpaceState::MovementOnSpline state.*/
	UPROPERTY(EditAnywhere, Category = "Config", meta = (EditCondition = "MovementSpaceSpawn == EMovementSpaceState::MovementOnSpline", EditConditionHides))
	TWeakObjectPtr<AActor> MovementSplineActorSpawn{nullptr};

	/** Reference to the USplineComponent of the Actor in the world which holds the USplineComponent on which the player can move, if in the EMovementSpaceState::MovementOnSpline state.*/
	UPROPERTY()
	TWeakObjectPtr<USplineComponent> MovementSplineComponentFromWorld{nullptr};
};