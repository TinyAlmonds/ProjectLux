// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "Core/Component/ChaseActorAlongSpline/PLChaseActorAlongSplineSettings.h"
#include "PLChaseActorAlongSplineComponent.generated.h"

// Forward declarations
class USplineComponent;

/**
 * ActorComponent class to chase actors or the player actor along a spline.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTLUX_API UPLChaseActorAlongSplineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties. */
	UPLChaseActorAlongSplineComponent();

	/** The tick method called every frame. */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	/**
	 * Returns the ChaseActorAlongSplineActorMode state.
	 * @return The ChaseActorAlongSplineActorMode state.
	 */
	UFUNCTION(BlueprintCallable, Category = "")
	EPLChaseActorAlongSplineActorMode GetActorMode() const;

	/**
	 * Sets the ChaseActorAlongSplineActorMode state.
	 * @param Mode - The ChaseActorAlongSplineActorMode state to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "")
	void SetActorMode(EPLChaseActorAlongSplineActorMode Mode);

	/**
	 * Returns the chased actor.
	 * @return The chased actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "")
	const AActor *GetChasedActor() const;

	/**
	 * Sets the chased actor.
	 * @param ActorToChase - The chased actor to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "")
	void SetChaseActor(AActor *ActorToChase);

protected:
	/** Method called when the game starts. */
	virtual void BeginPlay() override;

private:
	/** The settings of the chase. */
	UPROPERTY(EditAnywhere, Category = "Config")
	FPLChaseActorAlongSplineSet ChaseActorSettings{};

	/** The previously used ChaseActorAlongSplineActorMode state. */
	EPLChaseActorAlongSplineActorMode PreviousActorMode{EPLChaseActorAlongSplineActorMode::Actor};

	/** The SplineComponent used to chase the target along. */
	TWeakObjectPtr<USplineComponent> SplineToChaseAlong{};

	/** The reference SplineComponent used for EPLChaseActorAlongSplineChaseMode::FollowPositionOnReferenceSpline. */
	TWeakObjectPtr<USplineComponent> ReferenceSplineToFollow{};

	/** Method used when the ChaseActorAlongSplineActorMode changed. */
	void ActorModeChanged();

	/** Method for setting the player as the chased actor. */
	void SetChaseActorForPlayerActorMode();
};
